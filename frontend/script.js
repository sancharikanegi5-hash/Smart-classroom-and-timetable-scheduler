let currentResultData = null;

let activeSectionIndex = 0;

const DEFAULT_SLOTS = [
    { start: "09:00", end: "09:55" },
    { start: "09:55", end: "10:50" },
    { start: "11:00", end: "11:55" },
    { start: "11:55", end: "12:50" },
    { start: "12:50", end: "01:45" },
    { start: "01:45", end: "02:40" },
    { start: "03:00", end: "03:55" },
    { start: "03:55", end: "04:50" },
    { start: "04:50", end: "05:45" },
    { start: "05:45", end: "06:40" }
];

const TABLE_NAMES = ['sections', 'faculty', 'rooms', 'subjects', 'assignments'];

function getElement(selector, parent = document) {
    return parent.querySelector(selector);
}

function getAllElements(selector, parent = document) {
    return Array.from(parent.querySelectorAll(selector));
}

function escapeHtml(text) {
    const replacements = {
        '&': '&amp;',
        '<': '&lt;',
        '>': '&gt;',
        '"': '&quot;',
        "'": '&#39;'
    };
    return String(text || '').replace(/[&<>"']/g, character => replacements[character]);
}

function switchPortalTab(tabId) {
    getAllElements('.tab-panel').forEach(panel => {
        panel.classList.toggle('active', panel.id === tabId);
    });
    getAllElements('.nav-tab').forEach(tabButton => {
        tabButton.classList.toggle('active', tabButton.dataset.tab === tabId);
    });
    window.scrollTo(0, 0);
}

function showSection(id) {
    if (id === 'tab-view' || id === 'view-section') {
        switchPortalTab('tab-view');
    } else {
        switchPortalTab('tab-institution');
    }
}

function updateTabBadges() {
    TABLE_NAMES.forEach(name => {
        const badge = getElement(`#badge-${name}`);
        if (badge) {
            badge.textContent = getAllElements(`#${name}-body tr`).length;
        }
    });
}

function updateTimeSlotInputs() {
    const periodCount = parseInt(getElement('#periodsPerDay').value) || 8;
    const container = getElement('#timeslot-inputs');
    container.innerHTML = '';

    for (let i = 0; i < periodCount; i++) {

        const slot = DEFAULT_SLOTS[i] || { start: "09:00", end: "10:00" };

        const item = document.createElement('div');
        item.className = 'timeslot-compact-item';
        item.innerHTML = `
            <label>Period ${i + 1}</label>
            <div class="timeslot-item-inputs">
                <span>From:</span><input type="text" class="ts-start" value="${slot.start}">
                <span>To:</span><input type="text" class="ts-end" value="${slot.end}">
            </div>`;
        container.appendChild(item);
    }
}

function readTimeSlotLabels(separator) {
    const startInputs = getAllElements('.ts-start');
    const endInputs = getAllElements('.ts-end');

    return startInputs.map((startInput, index) => {
        const start = startInput.value.trim();
        const end = endInputs[index]?.value.trim();

        if (start && end) {
            return start + separator + end;
        }
        return `Period ${index + 1}`;
    });
}

function appendTableRow(tbodyId, rowClass, html) {
    const newRow = document.createElement('tr');
    newRow.className = rowClass;
    newRow.innerHTML = html;
    getElement(`#${tbodyId}`).appendChild(newRow);
    return newRow;
}

function removeRow(button) {
    button.closest('tr')?.remove();
    refreshAssignmentDropdowns();
    updateTabBadges();
}

function addSectionRow(sectionData) {

    const existingCount = getAllElements('#sections-body tr').length;
    const defaultName = existingCount < 26
        ? String.fromCharCode(65 + existingCount)
        : `Sec${existingCount + 1}`;

    const rowData = {
        name: sectionData?.name !== undefined ? sectionData.name : defaultName,
        course: sectionData?.course || '',
        semester: sectionData?.semester || '',
        coordinator: sectionData?.coordinator || '',
        strength: sectionData?.studentStrength || sectionData?.strength || 60
    };

    appendTableRow('sections-body', 'section-row', `
        <td><input type="text" class="sec-name" value="${escapeHtml(rowData.name)}" placeholder="e.g. A, B" required oninput="refreshAssignmentDropdowns()"></td>
        <td><input type="text" class="sec-course" value="${escapeHtml(rowData.course)}" placeholder="e.g. B.Tech CSE"></td>
        <td><input type="text" class="sec-sem" value="${escapeHtml(rowData.semester)}" placeholder="e.g. III"></td>
        <td><input type="number" class="sec-strength" value="${rowData.strength}" min="1" max="500" placeholder="60" oninput="refreshAssignmentDropdowns()"></td>
        <td><input type="text" class="sec-coord" value="${escapeHtml(rowData.coordinator)}" placeholder="e.g. Dr. Coordinator"></td>
        <td style="text-align:center;"><button type="button" class="btn btn-remove" onclick="removeRow(this)">Remove</button></td>
    `);

    refreshAssignmentDropdowns();
    updateTabBadges();
}

function addFacultyRow(facultyData) {
    const rowData = facultyData || { id: '', name: '', dept: '' };

    appendTableRow('faculty-body', 'faculty-row', `
        <td><input type="text" class="fac-id" value="${escapeHtml(rowData.id)}" placeholder="e.g. F1"></td>
        <td><input type="text" class="fac-name" value="${escapeHtml(rowData.name)}" placeholder="e.g. Dr. Name" required oninput="refreshAssignmentDropdowns()"></td>
        <td><input type="text" class="fac-dept" value="${escapeHtml(rowData.dept)}" placeholder="e.g. CSE"></td>
        <td style="text-align:center;"><button type="button" class="btn btn-remove" onclick="removeRow(this)">Remove</button></td>
    `);

    refreshAssignmentDropdowns();
    updateTabBadges();
}

function addRoomRow(roomData) {
    const rowData = roomData || { id: '', type: 'Classroom', capacity: 60 };

    appendTableRow('rooms-body', 'room-row', `
        <td><input type="text" class="room-id" value="${escapeHtml(rowData.id)}" placeholder="e.g. Room 101" required oninput="refreshAssignmentDropdowns()"></td>
        <td>
            <select class="room-type">
                <option value="Classroom" ${rowData.type === 'Classroom' ? 'selected' : ''}>Classroom</option>
                <option value="Laboratory" ${rowData.type === 'Laboratory' ? 'selected' : ''}>Laboratory</option>
            </select>
        </td>
        <td><input type="number" class="room-cap" value="${rowData.capacity || 60}" min="10" max="300"></td>
        <td style="text-align:center;"><button type="button" class="btn btn-remove" onclick="removeRow(this)">Remove</button></td>
    `);

    refreshAssignmentDropdowns();
    updateTabBadges();
}

function addSubjectRow(subjectData) {
    const rowData = subjectData || { code: '', name: '', hours: 3, isLab: false, category: 'CORE' };

    appendTableRow('subjects-body', 'subject-row', `
        <td><input type="text" class="sub-code" value="${escapeHtml(rowData.code)}" placeholder="e.g. CS301" required oninput="refreshAssignmentDropdowns()"></td>
        <td><input type="text" class="sub-name" value="${escapeHtml(rowData.name)}" placeholder="e.g. Algorithms" required oninput="refreshAssignmentDropdowns()"></td>
        <td><input type="number" class="sub-hours" value="${rowData.hours || 3}" min="1" max="10"></td>
        <td>
            <select class="sub-islab" onchange="onSubjectTypeChange(this)">
                <option value="false" ${!rowData.isLab ? 'selected' : ''}>Theory</option>
                <option value="true" ${rowData.isLab ? 'selected' : ''}>Laboratory (2 slots)</option>
            </select>
        </td>
        <td>
            <select class="sub-cat">
                <option value="CORE" ${rowData.category === 'CORE' ? 'selected' : ''}>CORE</option>
                <option value="ELECTIVE" ${rowData.category === 'ELECTIVE' ? 'selected' : ''}>ELECTIVE</option>
                <option value="LABS" ${rowData.category === 'LABS' ? 'selected' : ''}>LABS</option>
            </select>
        </td>
        <td style="text-align:center;"><button type="button" class="btn btn-remove" onclick="removeRow(this)">Remove</button></td>
    `);

    refreshAssignmentDropdowns();
    updateTabBadges();
}

function onSubjectTypeChange(dropdown) {
    const tableRow = dropdown.closest('tr');
    if (dropdown.value === 'true') {
        getElement('.sub-cat', tableRow).value = 'LABS';
    }
}

function addAssignmentRow(assignmentData) {
    const rowData = assignmentData || {
        subjectCode: '', section: '', sections: [],
        isCombined: false, faculty: '', hours: 3, isLab: false, category: 'CORE'
    };

    const isCombined = !!(rowData.isCombined || (rowData.sections && rowData.sections.length > 1));

    let selectedSections = [];
    if (rowData.sections && rowData.sections.length > 0) {
        selectedSections = rowData.sections;
    } else if (rowData.section) {
        selectedSections = [rowData.section];
    }

    const newRow = appendTableRow('assignments-body', 'assignment-row', `
        <td>
            <select class="assign-type" onchange="onAssignTypeChange(this)">
                <option value="individual" ${!isCombined ? 'selected' : ''}>Individual Section</option>
                <option value="combined" ${isCombined ? 'selected' : ''}>Combined Sections</option>
            </select>
        </td>
        <td>
            <select class="assign-subject" onchange="onAssignmentSubjectChange(this)">
                ${getSubjectOptionsHtml(rowData.subjectCode)}
            </select>
        </td>
        <td>
            <div class="section-single-container ${isCombined ? 'hidden' : ''}">
                <select class="assign-section">
                    ${getSectionOptionsHtml(rowData.section || selectedSections[0] || '')}
                </select>
            </div>
            <div class="section-combined-container ${!isCombined ? 'hidden' : ''}">
                ${getSectionCheckboxesHtml(selectedSections)}
            </div>
        </td>
        <td>
            <select class="assign-faculty">
                ${getFacultyOptionsHtml(rowData.faculty)}
            </select>
        </td>
        <td><input type="number" class="assign-hours" value="${rowData.hours || 3}" min="1" max="10" style="width:65px;"></td>
        <td>
            <select class="assign-islab">
                <option value="false" ${!rowData.isLab ? 'selected' : ''}>Lecture</option>
                <option value="true" ${rowData.isLab ? 'selected' : ''}>Lab (2 Slots)</option>
            </select>
            <input type="hidden" class="assign-cat" value="${rowData.category || 'CORE'}">
        </td>
        <td style="text-align:center;"><button type="button" class="btn btn-remove" onclick="removeRow(this)">Remove</button></td>
    `);

    if (isCombined) {
        const firstCheckbox = getElement('.cb-section', newRow);
        if (firstCheckbox) {
            updateCombinedStrengthHint(firstCheckbox);
        }
    }

    updateTabBadges();
}

function onAssignTypeChange(dropdown) {
    const tableRow = dropdown.closest('tr');
    const isCombined = (dropdown.value === 'combined');

    getElement('.section-single-container', tableRow).classList.toggle('hidden', isCombined);
    getElement('.section-combined-container', tableRow).classList.toggle('hidden', !isCombined);

    const firstCheckbox = getElement('.cb-section', tableRow);
    if (firstCheckbox) {
        updateCombinedStrengthHint(firstCheckbox);
    }
}

function getRegisteredSections() {
    const sections = getAllElements('#sections-body tr').map(row => ({
        name: getElement('.sec-name', row).value.trim(),
        strength: parseInt(getElement('.sec-strength', row).value) || 60
    }));

    return sections.filter(section => section.name);
}

function getSubjectOptionsHtml(selectedCode) {
    const subjects = getAllElements('#subjects-body tr').map(row => ({
        code: getElement('.sub-code', row).value.trim(),
        name: getElement('.sub-name', row).value.trim(),
        hours: getElement('.sub-hours', row).value,
        isLab: getElement('.sub-islab', row).value,
        category: getElement('.sub-cat', row).value
    })).filter(subject => subject.code);

    let html = '<option value="">-- Select Subject --</option>';

    subjects.forEach(subject => {
        const isSelected = (subject.code === selectedCode) ? 'selected' : '';

        html += `<option value="${escapeHtml(subject.code)}"
                    data-name="${escapeHtml(subject.name)}"
                    data-hours="${subject.hours}"
                    data-islab="${subject.isLab}"
                    data-cat="${subject.category}"
                    ${isSelected}>${escapeHtml(subject.code)} - ${escapeHtml(subject.name)}</option>`;
    });

    const stillExists = subjects.some(subject => subject.code === selectedCode);
    if (selectedCode && !stillExists) {
        html += `<option value="${escapeHtml(selectedCode)}" selected>${escapeHtml(selectedCode)}</option>`;
    }

    return html;
}

function getSectionOptionsHtml(selectedName) {
    const sections = getRegisteredSections();
    let html = '<option value="">-- Section --</option>';

    sections.forEach(section => {
        const isSelected = (section.name === selectedName) ? 'selected' : '';
        html += `<option value="${escapeHtml(section.name)}" ${isSelected}>Section ${escapeHtml(section.name)} (${section.strength} students)</option>`;
    });

    const stillExists = sections.some(section => section.name === selectedName);
    if (selectedName && !stillExists) {
        html += `<option value="${escapeHtml(selectedName)}" selected>Section ${escapeHtml(selectedName)}</option>`;
    }

    return html;
}

function getSectionCheckboxesHtml(selectedList = []) {
    const sections = getRegisteredSections();

    if (sections.length === 0) {
        return '<span style="color:#94a3b8;font-size:11px;">No sections added</span>';
    }

    const selectedSet = new Set(selectedList);
    let boxesHtml = '';

    sections.forEach(section => {
        const isChecked = selectedSet.has(section.name) ? 'checked' : '';
        boxesHtml += `
            <label class="cb-label">
                <input type="checkbox" class="cb-section"
                       value="${escapeHtml(section.name)}"
                       data-strength="${section.strength}"
                       onchange="updateCombinedStrengthHint(this)" ${isChecked}>
                Sec ${escapeHtml(section.name)} (${section.strength})
            </label>`;
    });

    return `<div class="combined-picker">${boxesHtml}</div><div class="combined-strength-hint"></div>`;
}

function updateCombinedStrengthHint(element) {
    const cell = element.closest('td');
    const hint = cell?.querySelector('.combined-strength-hint');
    if (!hint) return;

    const checkedBoxes = getAllElements('.cb-section:checked', cell);

    let totalStudents = 0;
    const sectionNames = [];
    checkedBoxes.forEach(checkbox => {
        totalStudents += parseInt(checkbox.dataset.strength) || 60;
        sectionNames.push(checkbox.value);
    });

    if (sectionNames.length > 0) {
        hint.textContent = `Combined (${sectionNames.join('+')}): ${totalStudents} students total`;
    } else {
        hint.textContent = 'Select sections';
    }
}

function getFacultyOptionsHtml(selectedName) {
    const facultyNames = getAllElements('#faculty-body tr')
        .map(row => getElement('.fac-name', row).value.trim())
        .filter(Boolean);

    let html = '<option value="">-- Select Faculty --</option>';

    facultyNames.forEach(name => {
        const isSelected = (name === selectedName) ? 'selected' : '';
        html += `<option value="${escapeHtml(name)}" ${isSelected}>${escapeHtml(name)}</option>`;
    });

    if (selectedName && !facultyNames.includes(selectedName)) {
        html += `<option value="${escapeHtml(selectedName)}" selected>${escapeHtml(selectedName)}</option>`;
    }

    return html;
}

function onAssignmentSubjectChange(dropdown) {
    const tableRow = dropdown.closest('tr');
    const chosenOption = dropdown.selectedOptions[0];

    if (chosenOption?.dataset) {
        if (chosenOption.dataset.hours) {
            getElement('.assign-hours', tableRow).value = chosenOption.dataset.hours;
        }
        if (chosenOption.dataset.islab) {
            getElement('.assign-islab', tableRow).value = chosenOption.dataset.islab;
        }
        if (chosenOption.dataset.cat) {
            getElement('.assign-cat', tableRow).value = chosenOption.dataset.cat;
        }
    }
}

function refreshAssignmentDropdowns() {
    getAllElements('#assignments-body tr').forEach(row => {

        const subjectDropdown = getElement('.assign-subject', row);
        const currentSubject = subjectDropdown.value;
        subjectDropdown.innerHTML = getSubjectOptionsHtml(currentSubject);

        const sectionDropdown = getElement('.assign-section', row);
        const currentSection = sectionDropdown.value;
        sectionDropdown.innerHTML = getSectionOptionsHtml(currentSection);

        const facultyDropdown = getElement('.assign-faculty', row);
        const currentFaculty = facultyDropdown.value;
        facultyDropdown.innerHTML = getFacultyOptionsHtml(currentFaculty);

        const combinedContainer = getElement('.section-combined-container', row);
        if (combinedContainer) {
            const checkedNames = getAllElements('.cb-section:checked', combinedContainer)
                .map(checkbox => checkbox.value);
            combinedContainer.innerHTML = getSectionCheckboxesHtml(checkedNames);

            const firstCheckbox = getElement('.cb-section', combinedContainer);
            if (firstCheckbox) {
                updateCombinedStrengthHint(firstCheckbox);
            }
        }
    });
}

function clearAllTables() {
    TABLE_NAMES.forEach(name => {
        getElement(`#${name}-body`).innerHTML = '';
    });
    updateTabBadges();
}

function initBlankForm() {
    clearAllTables();

    ['universityName', 'departmentName', 'courseName', 'academicYear', 'wefDate'].forEach(id => {
        const inputBox = getElement(`#${id}`);
        if (inputBox) inputBox.value = '';
    });

    getElement('#workingDays').value = '5';
    getElement('#periodsPerDay').value = '8';
    getElement('#breakAfterPeriod').value = '6';

    updateTimeSlotInputs();
    addSectionRow();
    updateTabBadges();
}

function formHasData() {
    const hasUniversityName = getElement('#universityName').value.trim() !== '';
    const hasManySections = getAllElements('#sections-body tr').length > 1;
    const hasFaculty = getAllElements('#faculty-body tr').length > 0;
    const hasSubjects = getAllElements('#subjects-body tr').length > 0;
    const hasAssignments = getAllElements('#assignments-body tr').length > 0;

    return hasUniversityName || hasManySections || hasFaculty || hasSubjects || hasAssignments;
}

function loadSampleDataWithConfirmation() {
    if (formHasData()) {
        const userSaidYes = confirm('Load sample demonstration data? Your current inputs will be replaced.');
        if (!userSaidYes) return;
    }
    loadGraphicEraSampleData();
}

function loadGraphicEraSampleData() {
    fetch('/api/sample')
        .then(response => {
            if (!response.ok) {
                throw new Error(`Server returned ${response.status}`);
            }
            return response.json();
        })
        .then(sampleConfig => {
            applyConfigToForm(sampleConfig);
            switchPortalTab('tab-assignments');
        })
        .catch(error => alert('Could not load sample data: ' + error.message));
}

function collectCurrentConfig() {
    const config = {

        universityName: getElement('#universityName').value.trim(),
        departmentName: getElement('#departmentName').value.trim(),
        courseName: getElement('#courseName')?.value.trim() || '',
        academicYear: getElement('#academicYear')?.value.trim() || '',
        wefDate: getElement('#wefDate').value.trim(),

        workingDays: parseInt(getElement('#workingDays').value) || 5,
        periodsPerDay: parseInt(getElement('#periodsPerDay').value) || 8,
        breakAfterPeriod: parseInt(getElement('#breakAfterPeriod').value) || 0,
        timeSlotLabels: readTimeSlotLabels(' - '),

        sections: getAllElements('#sections-body tr').map(row => ({
            name: getElement('.sec-name', row).value.trim(),
            course: getElement('.sec-course', row).value.trim(),
            semester: getElement('.sec-sem', row).value.trim(),
            studentStrength: parseInt(getElement('.sec-strength', row).value) || 60,
            coordinator: getElement('.sec-coord', row).value.trim()
        })),

        faculty: getAllElements('#faculty-body tr').map(row => ({
            id: getElement('.fac-id', row).value.trim(),
            name: getElement('.fac-name', row).value.trim(),
            dept: getElement('.fac-dept', row).value.trim()
        })),

        rooms: getAllElements('#rooms-body tr').map(row => ({
            id: getElement('.room-id', row).value.trim(),
            type: getElement('.room-type', row).value,
            capacity: parseInt(getElement('.room-cap', row).value) || 60
        })),

        subjects: getAllElements('#subjects-body tr').map(row => ({
            code: getElement('.sub-code', row).value.trim(),
            name: getElement('.sub-name', row).value.trim(),
            hours: parseInt(getElement('.sub-hours', row).value) || 3,
            isLab: getElement('.sub-islab', row).value === 'true',
            category: getElement('.sub-cat', row).value
        })),

        assignments: getAllElements('#assignments-body tr').map(row => {
            const isCombined = getElement('.assign-type', row).value === 'combined';

            let checkedSections = [];
            if (isCombined) {
                checkedSections = getAllElements('.cb-section:checked', row).map(box => box.value);
            }

            let sectionName;
            if (isCombined) {
                sectionName = checkedSections[0] || '';
            } else {
                sectionName = getElement('.assign-section', row).value.trim();
            }

            return {
                subjectCode: getElement('.assign-subject', row).value.trim(),
                sectionName: sectionName,
                isCombined: isCombined,
                combinedLabel: (isCombined && checkedSections.length) ? `Combined ${checkedSections.join('+')}` : '',
                sections: isCombined ? checkedSections : [sectionName],
                facultyName: getElement('.assign-faculty', row).value.trim(),
                hoursPerWeek: parseInt(getElement('.assign-hours', row).value) || 3,
                isLab: getElement('.assign-islab', row).value === 'true',
                category: getElement('.assign-cat', row).value || 'CORE'
            };
        })
    };

    return config;
}

function saveConfigToFile() {
    const config = collectCurrentConfig();

    const rawName = config.universityName || 'timetable_config';
    const fileName = rawName.replace(/[^a-zA-Z0-9]/g, '_').toLowerCase();

    const fileBlob = new Blob([JSON.stringify(config, null, 2)], { type: 'application/json' });
    const downloadLink = document.createElement('a');
    downloadLink.href = URL.createObjectURL(fileBlob);
    downloadLink.download = `${fileName}.json`;
    downloadLink.click();
}

function loadConfigFromFile(fileInput) {
    const file = fileInput.files[0];
    if (!file) return;

    if (formHasData()) {
        const userSaidYes = confirm('Loading file will replace current inputs. Continue?');
        if (!userSaidYes) {
            fileInput.value = '';
            return;
        }
    }

    const reader = new FileReader();
    reader.onload = event => {
        try {
            const config = JSON.parse(event.target.result);
            applyConfigToForm(config);
            switchPortalTab('tab-assignments');
        } catch (error) {
            alert('Invalid config: ' + error.message);
        }
        fileInput.value = '';
    };
    reader.readAsText(file);
}

function applyConfigToForm(config) {
    clearAllTables();

    getElement('#universityName').value = config.universityName || '';
    getElement('#departmentName').value = config.departmentName || '';
    if (getElement('#courseName')) getElement('#courseName').value = config.courseName || '';
    if (getElement('#academicYear')) getElement('#academicYear').value = config.academicYear || '';
    getElement('#wefDate').value = config.wefDate || '';

    getElement('#workingDays').value = String(config.workingDays || 5);
    getElement('#periodsPerDay').value = String(config.periodsPerDay || 8);
    getElement('#breakAfterPeriod').value = String(config.breakAfterPeriod || 0);

    updateTimeSlotInputs();
    if (config.timeSlotLabels) {
        const startInputs = getAllElements('.ts-start');
        const endInputs = getAllElements('.ts-end');

        config.timeSlotLabels.forEach((label, index) => {
            if (index < startInputs.length) {
                const parts = label.split(' - ');
                if (parts.length === 2) {
                    startInputs[index].value = parts[0].trim();
                    endInputs[index].value = parts[1].trim();
                }
            }
        });
    }

    (config.sections || []).forEach(section => addSectionRow(section));
    (config.faculty || []).forEach(faculty => addFacultyRow(faculty));
    (config.rooms || []).forEach(room => addRoomRow(room));
    (config.subjects || []).forEach(subject => addSubjectRow(subject));

    (config.assignments || []).forEach(assignment => {
        addAssignmentRow({
            subjectCode: assignment.subjectCode || assignment.code || '',
            section: assignment.sectionName || assignment.section || '',
            sections: assignment.sections || [],
            isCombined: assignment.isCombined || false,
            faculty: assignment.facultyName || assignment.faculty || '',
            hours: assignment.hoursPerWeek || assignment.hours || 3,
            isLab: assignment.isLab || false,
            category: assignment.category || 'CORE'
        });
    });

    if (!config.sections?.length) {
        addSectionRow();
    }
    updateTabBadges();
}

function handleFormSubmit(event) {
    event.preventDefault();
    const config = collectCurrentConfig();

    if (!config.sections.length || !config.sections[0].name) {
        alert('Please add at least one named Section.');
        return switchPortalTab('tab-sections');
    }

    if (!config.assignments.length) {
        alert('Please add at least one Class Assignment.');
        return switchPortalTab('tab-assignments');
    }

    const subjectNameByCode = {};
    config.subjects.forEach(subject => {
        subjectNameByCode[subject.code] = subject.name;
    });

    for (let i = 0; i < config.assignments.length; i++) {
        const assignment = config.assignments[i];

        if (!assignment.subjectCode) {
            alert(`Assignment #${i + 1} must have a subject selected.`);
            return switchPortalTab('tab-assignments');
        }
        if (assignment.isCombined && (!assignment.sections || assignment.sections.length < 1)) {
            alert(`Assignment #${i + 1} is marked as Combined. Select at least one section.`);
            return switchPortalTab('tab-assignments');
        }
        if (!assignment.isCombined && !assignment.sectionName) {
            alert(`Assignment #${i + 1} must have a section selected.`);
            return switchPortalTab('tab-assignments');
        }

        assignment.subjectName = subjectNameByCode[assignment.subjectCode] || assignment.subjectCode;
        assignment.facultyName = assignment.facultyName || 'TBD';
        assignment.roomName = 'TBD';
    }

    config.timeSlotLabels = readTimeSlotLabels(' -\n');

    getElement('#loading').classList.remove('hidden');

    fetch('/generate', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(config)
    })
    .then(response => response.json())
    .then(result => {
        getElement('#loading').classList.add('hidden');

        if (result.success && result.sections?.length) {
            currentResultData = result;
            renderMultiSectionView(result);
            switchPortalTab('tab-view');
        } else {
            alert('Scheduling Failed:\n\n' + (result.error || 'Could not satisfy all constraints.'));
        }
    })
    .catch(error => {
        getElement('#loading').classList.add('hidden');
        alert('Server communication error: ' + error.message);
    });
}

function renderMultiSectionView(data) {
    const tabsContainer = getElement('#section-tabs');
    tabsContainer.innerHTML = '';
    activeSectionIndex = 0;

    data.sections.forEach((section, index) => {
        const tabButton = document.createElement('button');
        tabButton.type = 'button';
        tabButton.className = 'section-tab';
        if (index === 0) tabButton.classList.add('active');
        tabButton.textContent = `Section ${section.sectionName}`;
        tabButton.onclick = () => switchSectionTab(index);
        tabsContainer.appendChild(tabButton);
    });

    renderSectionTimetable(0);
}

function switchSectionTab(index) {
    activeSectionIndex = index;
    getAllElements('.section-tab').forEach((tabButton, tabIndex) => {
        tabButton.classList.toggle('active', tabIndex === index);
    });
    renderSectionTimetable(index);
}

function renderSectionTimetable(index) {
    if (!currentResultData?.sections[index]) return;

    const section = currentResultData.sections[index];
    getElement('#rendered-timetable-content').innerHTML = buildSectionHtml(section, currentResultData);
}

function buildHeaderColumns(timeSlots, breakAfter) {
    let html = '';

    timeSlots.forEach((timeSlot, index) => {

        if (breakAfter > 0 && index === breakAfter) {
            html += '<th style="background:#cbd5e1;font-size:8pt;">LUNCH<br>BREAK</th>';
        }

        const formattedTime = timeSlot.replace(' -\n', '<br>-<br>').replace(' - ', '<br>-<br>');
        html += `<th>${formattedTime}</th>`;
    });

    return html;
}

function buildBodyRows(days, timeSlots, grid, breakAfter) {
    let allRowsHtml = '';

    days.forEach((dayName, dayIndex) => {
        const dayRow = grid[dayIndex] || [];
        let cellsHtml = '';

        for (let slotIndex = 0; slotIndex < timeSlots.length; slotIndex++) {

            if (breakAfter > 0 && slotIndex === breakAfter) {
                cellsHtml += '<td class="tt-break-cell">BREAK</td>';
            }

            const cell = dayRow[slotIndex];

            if (!cell) {
                cellsHtml += '<td class="empty-slot"></td>';
                continue;
            }

            const previousCell = dayRow[slotIndex - 1];
            const isSecondHalfOfLab = slotIndex > 0
                && previousCell?.subjectCode === cell.subjectCode
                && cell.isLab;
            if (isSecondHalfOfLab) continue;

            const nextCell = dayRow[slotIndex + 1];
            const isFirstHalfOfLab = cell.isLab
                && slotIndex + 1 < dayRow.length
                && nextCell?.subjectCode === cell.subjectCode;
            const colspanAttribute = isFirstHalfOfLab ? ' colspan="2" class="lab-merged"' : '';

            let combinedBadge = '';
            if (cell.isCombined && cell.combinedLabel) {
                combinedBadge = `<div class="cell-combined-badge">${escapeHtml(cell.combinedLabel)}</div>`;
            }

            let roomTag = '';
            if (cell.roomName && cell.roomName !== 'TBD') {
                roomTag = `<div class="cell-room-code">${escapeHtml(cell.roomName)}</div>`;
            }

            cellsHtml += `<td${colspanAttribute}>${combinedBadge}<div class="cell-subject-code">${escapeHtml(cell.subjectCode)}</div>${roomTag}</td>`;
        }

        allRowsHtml += `<tr><td class="row-day">${dayName}</td>${cellsHtml}</tr>`;
    });

    return allRowsHtml;
}

function buildSubjectRows(subjectList) {
    let html = '';

    subjectList.forEach(subject => {
        const room = (subject.roomName && subject.roomName !== 'TBD') ? subject.roomName : '-';

        let combinedBadge = '';
        if (subject.isCombined && subject.combinedLabel) {
            combinedBadge = ` <span class="cell-combined-badge">${escapeHtml(subject.combinedLabel)}</span>`;
        }

        html += `<tr>
            <td><strong>${escapeHtml(subject.subjectCode)}</strong></td>
            <td>${escapeHtml(subject.subjectName)}${combinedBadge}</td>
            <td>${escapeHtml(subject.facultyName)}</td>
            <td>${escapeHtml(room)}</td>
        </tr>`;
    });

    return html;
}

function buildSectionHtml(section, globalData) {
    const timeSlots = globalData.timeSlots || [];
    const days = globalData.days || ["MON", "TUE", "WED", "THU", "FRI"];
    const grid = section.timetable || [];
    const breakAfter = globalData.breakAfterPeriod || 0;

    const metaItems = [];
    if (section.courseName) metaItems.push(`<div><strong>Course:</strong> ${escapeHtml(section.courseName)}</div>`);
    if (section.semester) metaItems.push(`<div><strong>Semester:</strong> ${escapeHtml(section.semester)}</div>`);
    if (section.sectionName) metaItems.push(`<div><strong>Section:</strong> ${escapeHtml(section.sectionName)}</div>`);
    if (section.studentStrength) metaItems.push(`<div><strong>Strength:</strong> ${section.studentStrength}</div>`);
    if (globalData.wefDate) metaItems.push(`<div><strong>w. e. f.:</strong> ${escapeHtml(globalData.wefDate)}</div>`);
    const metaHtml = metaItems.join('');

    const headerColumnsHtml = buildHeaderColumns(timeSlots, breakAfter);
    const bodyRowsHtml = buildBodyRows(days, timeSlots, grid, breakAfter);

    const allSubjects = section.subjects || [];

    const coreSubjects = allSubjects.filter(subject =>
        !subject.isLab && subject.category !== 'LABS' && subject.category !== 'ELECTIVE');
    const electiveSubjects = allSubjects.filter(subject => subject.category === 'ELECTIVE');
    const labSubjects = allSubjects.filter(subject => subject.isLab || subject.category === 'LABS');

    let detailsBodyHtml = buildSubjectRows(coreSubjects);

    if (electiveSubjects.length > 0) {
        detailsBodyHtml += `<tr class="category-bar"><td colspan="4">ELECTIVE</td></tr>`;
        detailsBodyHtml += buildSubjectRows(electiveSubjects);
    }
    if (labSubjects.length > 0) {
        detailsBodyHtml += `<tr class="category-bar"><td colspan="4">LABS</td></tr>`;
        detailsBodyHtml += buildSubjectRows(labSubjects);
    }

    let coordinatorFooter = '';
    if (section.classCoordinator) {
        coordinatorFooter = `<div class="tt-coordinator-footer"><strong>CLASS CO-ORDINATOR:</strong> ${escapeHtml(section.classCoordinator)}</div>`;
    }

    return `
        <div class="tt-header">
            <h1 class="tt-institution">${escapeHtml(globalData.universityName || 'Institution Name')}</h1>
            <h2 class="tt-department">${escapeHtml(globalData.departmentName || 'Department')}</h2>
            <h3 class="tt-doc-title">CLASS TIME TABLE</h3>
            <div class="tt-meta-row">${metaHtml}</div>
        </div>

        <div class="tt-grid-wrapper">
            <table class="tt-table">
                <thead>
                    <tr><th class="col-day-time">DAY /<br>TIME</th>${headerColumnsHtml}</tr>
                </thead>
                <tbody>${bodyRowsHtml}</tbody>
            </table>
        </div>

        <div class="tt-details-wrapper">
            <table class="tt-details-table">
                <thead>
                    <tr>
                        <th style="width:130px;">SUBJECT CODE</th>
                        <th>SUBJECT NAME</th>
                        <th style="width:180px;">FACULTY NAME</th>
                        <th style="width:130px;">ROOM</th>
                    </tr>
                </thead>
                <tbody>${detailsBodyHtml}</tbody>
            </table>
        </div>

        ${coordinatorFooter}
    `;
}

function printCurrentTimetable() {
    getElement('#all-sections-print-container').classList.add('hidden');
    getElement('#single-section-container').classList.remove('hidden');
    window.print();
}

function printAllTimetables() {
    if (!currentResultData?.sections) return;

    const printContainer = getElement('#all-sections-print-container');
    const lastIndex = currentResultData.sections.length - 1;
    let allSheetsHtml = '';

    currentResultData.sections.forEach((section, index) => {

        const pageBreakClass = (index < lastIndex) ? 'page-break' : '';
        allSheetsHtml += `
            <div class="timetable-print-sheet ${pageBreakClass}">
                ${buildSectionHtml(section, currentResultData)}
            </div>`;
    });

    printContainer.innerHTML = allSheetsHtml;

    getElement('#single-section-container').classList.add('hidden');
    printContainer.classList.remove('hidden');
    window.print();

    setTimeout(() => {
        printContainer.classList.add('hidden');
        getElement('#single-section-container').classList.remove('hidden');
    }, 1000);
}

window.onload = function () {
    getElement('#timetable-form').addEventListener('submit', handleFormSubmit);
    initBlankForm();
};
