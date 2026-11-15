/*
 * Smart Classroom & Multi-Section Timetable Scheduler - Server
 *
 * Minimal Node.js HTTP server:
 *   1. Serves frontend HTML/CSS/JS and static assets.
 *   2. Receives multi-section scheduling configuration from browser.
 *   3. Uses unique temporary files per request for thread-safe concurrent scheduling.
 *   4. Invokes the C++ binary with [inputFile, outputFile] arguments.
 *   5. Returns the multi-section conflict-free timetables to the browser.
 *   6. Production-ready for Render deployment (0.0.0.0, process.env.PORT).
 */

const http = require('http');
const fs = require('fs');
const path = require('path');
const { execFile } = require('child_process');

const PORT = process.env.PORT || 3000;
const HOST = '0.0.0.0';

const SCHEDULER_PATH = process.platform === 'win32' && fs.existsSync(path.join(__dirname, 'scheduler.exe'))
    ? path.join(__dirname, 'scheduler.exe')
    : path.join(__dirname, 'scheduler');
const DATA_DIR = path.join(__dirname, 'data');

if (!fs.existsSync(DATA_DIR)) {
    fs.mkdirSync(DATA_DIR, { recursive: true });
}

const MIME_TYPES = {
    '.html': 'text/html',
    '.css': 'text/css',
    '.js': 'application/javascript',
    '.json': 'application/json',
    '.zip': 'application/zip',
    '.png': 'image/png',
    '.jpg': 'image/jpeg',
    '.svg': 'image/svg+xml'
};

const server = http.createServer(function(req, res) {

    // GET /api/sample - Return reference sample configuration
    if (req.method === 'GET' && (req.url === '/api/sample' || req.url === '/data/sample.json' || req.url === '/data/graphic_era_sample.json')) {
        const sampleFile = path.join(DATA_DIR, 'sample.json');
        fs.readFile(sampleFile, function(err, content) {
            if (err) {
                res.writeHead(404, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ error: 'Sample file not found' }));
            } else {
                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(content);
            }
        });
        return;
    }

    // POST /generate - Run the C++ multi-section scheduler
    if (req.method === 'POST' && req.url === '/generate') {
        let body = '';

        req.on('data', function(chunk) {
            body += chunk.toString();
        });

        req.on('end', function() {
            // Generate unique request ID to prevent file collisions during concurrent requests
            const reqId = Date.now() + '_' + Math.random().toString(36).substr(2, 7);
            const reqInputFile = path.join(DATA_DIR, 'input_' + reqId + '.json');
            const reqOutputFile = path.join(DATA_DIR, 'output_' + reqId + '.json');

            try {
                const inputData = JSON.parse(body);

                // Write unique request file for C++ engine
                fs.writeFileSync(reqInputFile, JSON.stringify(inputData, null, 2));

                // Also keep data/input.json updated for local testing/cli use
                try {
                    fs.writeFileSync(path.join(DATA_DIR, 'input.json'), JSON.stringify(inputData, null, 2));
                } catch (e) {}

                // Execute compiled C++ scheduler
                execFile(SCHEDULER_PATH, [reqInputFile, reqOutputFile], {
                    cwd: __dirname,
                    timeout: 15000   // 15 seconds timeout
                }, function(error, stdout, stderr) {
                    console.log('Scheduler output:\n', stdout);
                    if (stderr) console.error('Scheduler error output:\n', stderr);

                    if (error) {
                        const details = stderr.trim() || stdout.trim() || error.message;
                        res.writeHead(500, { 'Content-Type': 'application/json' });
                        res.end(JSON.stringify({
                            success: false,
                            error: 'Scheduler process failed: ' + details
                        }));
                        try { if (fs.existsSync(reqInputFile)) fs.unlinkSync(reqInputFile); } catch (e) {}
                        try { if (fs.existsSync(reqOutputFile)) fs.unlinkSync(reqOutputFile); } catch (e) {}
                        return;
                    }

                    try {
                        let outputJsonStr = '';
                        if (fs.existsSync(reqOutputFile)) {
                            outputJsonStr = fs.readFileSync(reqOutputFile, 'utf8');
                            // Also mirror to data/output.json for local cli review
                            try {
                                fs.writeFileSync(path.join(DATA_DIR, 'output.json'), outputJsonStr);
                            } catch (e) {}
                        } else {
                            throw new Error('Scheduler failed to produce output file');
                        }

                        const result = JSON.parse(outputJsonStr);
                        res.writeHead(200, { 'Content-Type': 'application/json' });
                        res.end(JSON.stringify(result));
                    } catch (readErr) {
                        res.writeHead(500, { 'Content-Type': 'application/json' });
                        res.end(JSON.stringify({
                            success: false,
                            error: 'Could not read scheduler results: ' + readErr.message
                        }));
                    } finally {
                        // Clean up temporary files
                        try { if (fs.existsSync(reqInputFile)) fs.unlinkSync(reqInputFile); } catch (e) {}
                        try { if (fs.existsSync(reqOutputFile)) fs.unlinkSync(reqOutputFile); } catch (e) {}
                    }
                });

            } catch (parseErr) {
                // Clean up on parse error
                try { if (fs.existsSync(reqInputFile)) fs.unlinkSync(reqInputFile); } catch (e) {}
                try { if (fs.existsSync(reqOutputFile)) fs.unlinkSync(reqOutputFile); } catch (e) {}

                res.writeHead(400, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({
                    success: false,
                    error: 'Invalid JSON request payload: ' + parseErr.message
                }));
            }
        });

        return;
    }

    // GET requests - serve static files
    let filePath = req.url.split('?')[0];
    if (filePath === '/' || filePath === '') {
        filePath = '/index.html';
    }

    let fullPath = path.join(__dirname, 'frontend', filePath);
    let ext = path.extname(fullPath).toLowerCase();
    let contentType = MIME_TYPES[ext] || 'text/plain';

    fs.readFile(fullPath, function(err, content) {
        if (err) {
            if (err.code === 'ENOENT') {
                res.writeHead(404, { 'Content-Type': 'text/plain' });
                res.end('Not Found: ' + filePath);
            } else {
                res.writeHead(500, { 'Content-Type': 'text/plain' });
                res.end('Server Error');
            }
        } else {
            res.writeHead(200, { 'Content-Type': contentType });
            res.end(content);
        }
    });
});

server.listen(PORT, HOST, function() {
    console.log('========================================================');
    console.log('  Smart Classroom & Multi-Section Timetable Scheduler');
    console.log('  Local:   http://localhost:' + PORT);
    console.log('  Network: http://127.0.0.1:' + PORT);
    console.log('========================================================');
});
