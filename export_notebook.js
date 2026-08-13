const fs = require('fs');
const path = require('path');

const notebookDir = path.join(__dirname, 'notebook');
const outputFile = path.join(__dirname, 'notebook.tex');

function escapeLatex(str) {
    return str
        .replace(/\\/g, '\\textbackslash{}')
        .replace(/[&%$#_{}]/g, m => '\\' + m)
        .replace(/~/g, '\\textasciitilde{}')
        .replace(/\^/g, '\\textasciicircum{}')
        .replace(/</g, '\\textless{}')
        .replace(/>/g, '\\textgreater{}')
        .replace(/\|/g, '\\textbar{}');
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


// Pull the public symbols out of a template so every one of them lands in the printed INDEX.
// That is what makes "where is minRectArea?" a two-second lookup instead of a page-flipping hunt.
function symbolsOf(code) {
    const out = new Set();
    const reType = /^\s*(?:template\s*<[^>]*>\s*)?(?:struct|class)\s+([A-Za-z_]\w*)/gm;
    const reFunc = /^(?:template\s*<[^>]*>\s*)?[A-Za-z_][\w:<>,\s*&]*?\b([a-zA-Z_]\w*)\s*\(/gm;
    let m;
    while ((m = reType.exec(code))) out.add(m[1]);
    while ((m = reFunc.exec(code))) out.add(m[1]);
    for (const bad of ['if', 'for', 'while', 'switch', 'return', 'sizeof', 'main', 'else', 'do'])
        out.delete(bad);
    return [...out];
}
function indexEntries(code) {
    return symbolsOf(code)
        .map(sym => `\\index{${sym.replace(/[_]/g, '\\_').replace(/[!@|"]/g, '"$&')}}`)
        .join('');
}
// First real comment line of a file = its one-line description, printed under the title.
function blurbOf(code) {
    for (const raw of code.split('\n')) {
        const l = raw.trim();
        if (!l) continue;
        if (l.startsWith('// Needs:') || l.startsWith('// NOTE:')) continue;
        if (l.startsWith('//')) return l.slice(2).trim();
        if (l.startsWith('/*')) return l.slice(2).replace(/^[=\s]+/, '').trim();
        return '';
    }
    return '';
}
function blurbLine(code) {
    const b = blurbOf(code);
    if (!b || b.length < 12) return '';
    const t = escapeLatex(b.length > 150 ? b.slice(0, 147) + '...' : b);
    return `\\nopagebreak{\\footnotesize\\sffamily\\color{blurb} ${t}\\par}\\nopagebreak\n`;
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
\\usepackage{makeidx}
\\usepackage{needspace}
\\usepackage[hidelinks]{hyperref}
\\makeindex

% Running head carries the SECTION on the left and the current TOPIC on the right, so you can
% thumb the printed notebook and always know where you are.
\\pagestyle{fancy}
\\fancyhf{}
\\fancyhead[L]{\\small\\sffamily\\bfseries\\leftmark}
\\fancyhead[R]{\\small\\sffamily\\rightmark}
\\fancyfoot[C]{\\sffamily\\thepage}
\\renewcommand{\\headrulewidth}{0.4pt}
\\renewcommand{\\sectionmark}[1]{\\markboth{#1}{}}
\\renewcommand{\\subsectionmark}[1]{\\markright{#1}}

\\definecolor{codebg}{HTML}{F8F9FA}
\\definecolor{codeframe}{HTML}{D0D5DD}
\\definecolor{keyword}{HTML}{0033CC}
\\definecolor{comment}{HTML}{008000}
\\definecolor{string}{HTML}{A31515}
\\definecolor{blurb}{HTML}{444444}

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
    numbers=none,
    xleftmargin=0.3em,
    framexleftmargin=0.3em,
    columns=fullflexible,
    keepspaces=true,
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
    {\\Huge \\bfseries Competitive Programming Notebook} \\par \\vspace{0.2em}
    {\\large ICPC team reference \\textbullet\\ \\today} \\par \\vspace{0.8em}
  \\end{center}
]

\\noindent\\fbox{\\parbox{\\linewidth}{\\footnotesize\\sffamily
\\textbf{HOW TO FIND SOMETHING (in order of speed).}
\\textbf{1.} You know the routine's name $\\rightarrow$ the alphabetical \\emph{Index of every
routine} at the very back gives the page.
\\textbf{2.} You know the topic $\\rightarrow$ the running head on every page names the section
(left) and the sub-topic (right); thumb until it matches.
\\textbf{3.} You know neither $\\rightarrow$ section \\textbf{12 Problem Solving} maps a problem's
\\emph{shape} and its \\emph{constraints} onto the technique, and \\texttt{docs/PLAYBOOK.md} does the
same at more length.
\\par\\vspace{0.4em}
\\textbf{BEFORE YOU PASTE.} Line 1 of every file says what it does; a \\texttt{// Needs:} line names
the templates it depends on; a \\texttt{// NOTE:} line warns when another file declares the same
symbol. Vertices and array positions are \\textbf{0-based} unless the header says otherwise.
Geometry: \\texttt{P\\textless ll\\textgreater} for every decision, \\texttt{P\\textless ld\\textgreater}\nonly when the answer is a real number.
}}
\\vspace{0.8em}

{\\small
\\setlength{\\cftbeforesecskip}{0.15em}
\\setlength{\\cftbeforesubsecskip}{0.02em}
\\setlength{\\cftsecindent}{0em}
\\setlength{\\cftsubsecindent}{0.8em}
\\renewcommand{\\cftsecfont}{\\bfseries\\sffamily}
\\renewcommand{\\cftsubsecfont}{\\sffamily}
\\renewcommand{\\cftsubsubsecfont}{\\sffamily\\footnotesize}
\\setcounter{tocdepth}{2}
\\tableofcontents
}
\\vspace{0.6em}

`;

// Warn loudly about anything that will be silently dropped from the PDF.
const skipped = [];
(function scan(dir) {
    for (const e of getSortedEntries(dir)) {
        if (e.isDir) scan(e.fullPath);
        else if (!e.name.endsWith('.cpp')) skipped.push(path.relative(notebookDir, e.fullPath));
    }
})(notebookDir);
if (skipped.length) {
    console.warn('WARNING: ' + skipped.length + ' file(s) are NOT .cpp and will be EXCLUDED from the notebook:');
    for (const f of skipped) console.warn('  - ' + f);
}

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
                latex += `\\needspace{4\\baselineskip}\n`;
                latex += `\\subsubsection{${escapeLatex(fileName)}}${indexEntries(code)}\n`;
                latex += blurbLine(code);
                latex += `\\begin{lstlisting}\n${code}\n\\end{lstlisting}\n\n`;
            }
        } else if (entry.name.endsWith('.cpp')) {
            const fileName = cleanName(path.basename(entry.name, '.cpp'));
            const code = fs.readFileSync(entry.fullPath, 'utf-8').trimEnd();
            latex += `\\needspace{4\\baselineskip}\n`;
            latex += `\\subsection{${escapeLatex(fileName)}}${indexEntries(code)}\n`;
            latex += blurbLine(code);
            latex += `\\begin{lstlisting}\n${code}\n\\end{lstlisting}\n\n`;
        }
    }
}

latex += `\\clearpage\n`;
latex += `\\phantomsection\\addcontentsline{toc}{section}{Index of every routine}\n`;
latex += `\\renewcommand{\\indexname}{Index of every routine (alphabetical)}\n`;
latex += `{\\footnotesize\\printindex}\n`;
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
