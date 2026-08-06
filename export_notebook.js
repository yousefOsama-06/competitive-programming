const fs = require('fs');
const path = require('path');

const notebookDir = path.join(__dirname, 'notebook');
const outputFile = path.join(__dirname, 'notebook.tex');

function escapeLatex(str) {
    return str
        .replace(/\\/g, '\\textbackslash{}')
        .replace(/[&%$#_{}~^]/g, m => '\\' + m);
}

function cleanName(name) {
    // Remove index prefix like "00 - "
    return name.replace(/^\d+\s*-\s*/, '');
}

function getSortedEntries(dirPath) {
    return fs.readdirSync(dirPath)
        .map(name => ({
            name,
            fullPath: path.join(dirPath, name),
            isDir: fs.statSync(path.join(dirPath, name)).isDirectory()
        }))
        .sort((a, b) => a.name.localeCompare(b.name));
}

let latex = `\\UseRawInputEncoding
\\documentclass[9pt,a4paper,twocolumn]{extarticle}
\\usepackage[left=0.5cm,right=0.5cm,top=0.7cm,bottom=0.7cm]{geometry}
\\usepackage{listings}
\\usepackage{xcolor}
\\usepackage{titlesec}
\\usepackage[T1]{fontenc}
\\usepackage[utf8]{inputenc}
\\usepackage{lmodern}
\\usepackage{fancyhdr}
\\usepackage{tocloft}
\\usepackage[hidelinks]{hyperref}

\\pagestyle{fancy}
\\fancyhf{}
\\fancyhead[C]{\\textbf{\\sffamily Competitive Programming Notebook}}
\\fancyfoot[C]{\\sffamily\\thepage}
\\renewcommand{\\headrulewidth}{0.4pt}

\\definecolor{codebg}{HTML}{F8F9FA}
\\definecolor{codeframe}{HTML}{D0D5DD}
\\definecolor{keyword}{HTML}{0033CC}
\\definecolor{comment}{HTML}{008000}
\\definecolor{string}{HTML}{A31515}

\\lstset{
    language=C++,
    basicstyle=\\ttfamily\\scriptsize,
    keywordstyle=\\color{keyword}\\bfseries,
    commentstyle=\\color{comment}\\itshape,
    stringstyle=\\color{string},
    backgroundcolor=\\color{codebg},
    frame=single,
    rulecolor=\\color{codeframe},
    breaklines=true,
    breakatwhitespace=false,
    tabsize=2,
    showstringspaces=false,
    numbers=left,
    numberstyle=\\tiny\\color{gray},
    numbersep=4pt,
    xleftmargin=0.6em,
    framexleftmargin=0.6em,
    aboveskip=0.3em,
    belowskip=0.3em,
    extendedchars=false,
}

\\titleformat{\\section}{\\large\\bfseries\\sffamily}{\\thesection}{0.4em}{}
\\titleformat{\\subsection}{\\normalsize\\bfseries\\sffamily}{\\thesubsection}{0.3em}{}
\\titleformat{\\subsubsection}{\\small\\bfseries\\sffamily}{\\thesubsubsection}{0.2em}{}

\\titlespacing*{\\section}{0pt}{0.5em}{0.2em}
\\titlespacing*{\\subsection}{0pt}{0.4em}{0.1em}
\\titlespacing*{\\subsubsection}{0pt}{0.3em}{0.1em}

\\setlength{\\columnseprule}{0pt}
\\setlength{\\columnsep}{0.6cm}

\\begin{document}

\\twocolumn[
  \\begin{center}
    {\\Huge \\bfseries Competitive Programming Notebook} \\par \\vspace{0.3em}
    {\\large \\today} \\par \\vspace{1em}
  \\end{center}
]

\\tableofcontents
\\vspace{1em}

`;

// Process top-level directories (sections)
const sections = getSortedEntries(notebookDir).filter(e => e.isDir);

for (const section of sections) {
    const sectionName = cleanName(section.name);
    latex += `\\section{${escapeLatex(sectionName)}}\n\n`;

    const entries = getSortedEntries(section.fullPath);

    // Process entries (files and subdirectories) together in sorted order
    for (const entry of entries) {
        if (entry.isDir) {
            const subdirName = cleanName(entry.name);
            latex += `\\subsection{${escapeLatex(subdirName)}}\n\n`;

            const subFiles = getSortedEntries(entry.fullPath).filter(e => !e.isDir && e.name.endsWith('.cpp'));
            for (const file of subFiles) {
                const fileName = cleanName(path.basename(file.name, '.cpp'));
                const code = fs.readFileSync(file.fullPath, 'utf-8').trimEnd();
                latex += `\\subsubsection{${escapeLatex(fileName)}}\n`;
                latex += `\\begin{lstlisting}\n${code}\n\\end{lstlisting}\n\n`;
            }
        } else if (entry.name.endsWith('.cpp')) {
            const fileName = cleanName(path.basename(entry.name, '.cpp'));
            const code = fs.readFileSync(entry.fullPath, 'utf-8').trimEnd();
            latex += `\\subsection{${escapeLatex(fileName)}}\n`;
            latex += `\\begin{lstlisting}\n${code}\n\\end{lstlisting}\n\n`;
        }
    }
}

latex += `\\end{document}\n`;

fs.writeFileSync(outputFile, latex, 'utf-8');
console.log('Generated: ' + outputFile);

// Generate toc.yaml
function generateTocYaml() {
    const yamlOutputFile = path.join(__dirname, 'toc.yaml');
    let yamlLines = [];
    yamlLines.push("# Competitive Programming Notebook Table of Contents");
    yamlLines.push("title: Competitive Programming Notebook");
    yamlLines.push("categories:");

    for (const section of sections) {
        yamlLines.push(`  - category: "${section.name}"`);
        yamlLines.push(`    title: "${cleanName(section.name)}"`);

        const entries = getSortedEntries(section.fullPath);
        const subdirs = entries.filter(e => e.isDir);
        const files = entries.filter(e => !e.isDir && e.name.endsWith('.cpp'));

        if (subdirs.length > 0) {
            yamlLines.push(`    subcategories:`);
            for (const sub of subdirs) {
                yamlLines.push(`      - name: "${sub.name}"`);
                yamlLines.push(`        title: "${cleanName(sub.name)}"`);
                const subFiles = getSortedEntries(sub.fullPath).filter(e => !e.isDir && e.name.endsWith('.cpp'));
                if (subFiles.length > 0) {
                    yamlLines.push(`        files:`);
                    for (const file of subFiles) {
                        yamlLines.push(`          - file: "${file.name}"`);
                        yamlLines.push(`            title: "${cleanName(path.basename(file.name, '.cpp'))}"`);
                        yamlLines.push(`            path: "${path.relative(__dirname, file.fullPath).replace(/\\/g, '/')}"`);
                    }
                } else {
                    yamlLines.push(`        files: []`);
                }
            }
        }

        if (files.length > 0) {
            yamlLines.push(`    files:`);
            for (const file of files) {
                yamlLines.push(`      - file: "${file.name}"`);
                yamlLines.push(`        title: "${cleanName(path.basename(file.name, '.cpp'))}"`);
                yamlLines.push(`        path: "${path.relative(__dirname, file.fullPath).replace(/\\/g, '/')}"`);
            }
        }
    }

    fs.writeFileSync(yamlOutputFile, yamlLines.join('\n'), 'utf-8');
    console.log('Generated: ' + yamlOutputFile);
}

generateTocYaml();

// Compile to PDF using pdflatex (run twice for ToC)
const { execSync } = require('child_process');
const os = require('os');
const texDir = path.dirname(outputFile);
const texFile = path.basename(outputFile);

// Find pdflatex - try PATH first, then common MiKTeX locations
function findPdflatex() {
    try {
        execSync('pdflatex --version', { stdio: 'pipe' });
        return 'pdflatex';
    } catch {}
    const home = os.homedir();
    const candidates = [
        path.join(home, 'AppData', 'Local', 'Programs', 'MiKTeX', 'miktex', 'bin', 'x64', 'pdflatex.exe'),
        'C:\\Program Files\\MiKTeX\\miktex\\bin\\x64\\pdflatex.exe',
    ];
    for (const c of candidates) {
        if (fs.existsSync(c)) return `"${c}"`;
    }
    throw new Error('pdflatex not found');
}

const pdflatex = findPdflatex();

try {
    console.log('Compiling PDF (pass 1/2)...');
    execSync(`${pdflatex} -interaction=nonstopmode -halt-on-error "${texFile}"`, { cwd: texDir, stdio: 'pipe' });
    console.log('Compiling PDF (pass 2/2)...');
    execSync(`${pdflatex} -interaction=nonstopmode -halt-on-error "${texFile}"`, { cwd: texDir, stdio: 'pipe' });

    const pdfFile = outputFile.replace('.tex', '.pdf');
    console.log('Generated: ' + pdfFile);

    // Clean up auxiliary files
    const baseName = path.basename(outputFile, '.tex');
    for (const ext of ['.aux', '.log', '.toc', '.out']) {
        const auxFile = path.join(texDir, baseName + ext);
        if (fs.existsSync(auxFile)) fs.unlinkSync(auxFile);
    }
    console.log('Cleaned up auxiliary files.');
} catch (err) {
    console.error('PDF compilation failed. Make sure pdflatex is installed and in PATH.');
    console.error(err.stderr?.toString() || err.message);
    process.exit(1);
}
