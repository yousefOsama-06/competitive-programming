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
    const noise = new Set(['if', 'for', 'while', 'switch', 'return', 'sizeof', 'main', 'else',
        'do', 'go', 'rec', 'dfs2', 'f', 'g', 'h', 'cmp', 'add', 'push', 'pull', 'init', 'build',
        'query', 'update', 'solve', 'test', 'sub', 'cur', 'ok', 'get', 'set', 'node', 'eye']);
    // 1-2 character names and generic helper names are noise in an alphabetical index
    return [...out].filter(x => x.length > 2 && !noise.has(x) && !x.startsWith('_'));
}
function indexEntries(code) {
    return symbolsOf(code)
        .map(sym => `\\index{${sym.replace(/[_]/g, '\\_').replace(/[!@|"]/g, '"$&')}}`)
        .join('');
}
// If the blurb reproduced line 1 of the file in full, drop that line from the listing - printing
// the same sentence twice on 200 files costs pages and reads as noise.
function trimBlurbLine(code) {
    const b = blurbOf(code);
    if (!b || b.length < 12 || b.length > 150) return code;
    const lines = code.split('\n');
    for (let i = 0; i < lines.length; i++) {
        const l = lines[i].trim();
        if (!l || l.startsWith('// Needs:') || l.startsWith('// NOTE:')) continue;
        if (l === '// ' + b || l === '//' + b) { lines.splice(i, 1); return lines.join('\n'); }
        return code;
    }
    return code;
}
// First real comment line of a file = its one-line description, printed under the title.
function blurbOf(code) {
    for (const raw of code.split('\n')) {
        const l = raw.trim();
        if (!l) continue;
        if (l.startsWith('// Needs:') || l.startsWith('// NOTE:')) continue;
        let t;
        if (l.startsWith('//')) t = l.slice(2);
        else if (l.startsWith('/*')) t = l.slice(2);
        else return '';
        // Banner comments are framed with runs of = or -. That framing is a divider in the
        // source and pure noise in a one-line caption, so strip it from both ends.
        return t.replace(/^[=*\s]{2,}/, '').replace(/[=*]{3,}\s*$/, '').trim();
    }
    return '';
}
function blurbLine(code) {
    const b = blurbOf(code);
    if (!b || b.length < 12) return '';
    const t = escapeLatex(b.length > 150 ? b.slice(0, 147) + '...' : b);
    return `\\nopagebreak{\\footnotesize\\sffamily\\raggedright\\color{blurb} ${t}\\par}\\nopagebreak\n`;
}

// ---------------------------------------------------------------------------------------------
// Stats for the title block. Computed from the tree, so they can never drift out of date.
function countTree(dir) {
    let files = 0, lines = 0;
    (function walk(d) {
        for (const e of getSortedEntries(d)) {
            if (e.isDir) walk(e.fullPath);
            else if (e.name.endsWith('.cpp')) {
                files++;
                lines += fs.readFileSync(e.fullPath, 'utf-8').split('\n').length;
            }
        }
    })(dir);
    return { files, lines };
}
const STATS = countTree(notebookDir);
const NSEC = getSortedEntries(notebookDir).filter(e => e.isDir).length;
const groupDigits = n => String(n).replace(/\B(?=(\d{3})+(?!\d))/g, '{,}');

const TITLE = 'Baskot Temp';
const AUTHOR = 'Husam Zaid';
const HANDLE = 'Baskot';

let latex = `\\UseRawInputEncoding
\\documentclass[9pt,a4paper,twocolumn]{extarticle}
\\usepackage[left=0.45cm,right=0.45cm,top=0.8cm,bottom=0.72cm,includeheadfoot,headsep=5pt,footskip=13pt]{geometry}
\\usepackage{listings}
\\usepackage{xcolor}
\\usepackage{titlesec}
\\usepackage[T1]{fontenc}
\\usepackage[utf8]{inputenc}
\\usepackage{lmodern}
% Inconsolata is ~15%% narrower than Latin Modern Typewriter, so 100-column source stops wrapping.
% Overleaf and any full TeX Live have it; a minimal install does not, so fall back gracefully.
\\IfFileExists{zi4.sty}{\\usepackage[varqu,varl]{zi4}}{%
  \\IfFileExists{inconsolata.sty}{\\usepackage[varqu,varl]{inconsolata}}{}}
\\IfFileExists{microtype.sty}{\\usepackage[verbose=silent]{microtype}}{}
\\IfFileExists{needspace.sty}{\\usepackage{needspace}}{\\newcommand{\\needspace}[1]{}}
\\usepackage{fancyhdr}
\\usepackage{multicol}
\\usepackage{makeidx}
\\usepackage[hidelinks]{hyperref}
\\IfFileExists{bookmark.sty}{\\usepackage{bookmark}}{}
\\makeindex

% ---------------------------------------------------------------------------------------------
% PALETTE. Chosen so the hierarchy survives a BLACK AND WHITE printer: ink -> accent -> rule runs
% dark -> mid -> light in LUMINANCE as well as in hue, so a greyscale print keeps every level.
\\definecolor{ink}{HTML}{101A24}        % near-black slate: section bars, heading text
\\definecolor{accent}{HTML}{1D5A82}     % deep petrol blue: numbers, rules, the section strip
\\definecolor{accentlt}{HTML}{7FA8C4}   % the same blue lightened, for use ON the dark bar
\\definecolor{rule}{HTML}{AEB8C2}       % hairlines and the code frame
\\definecolor{tint}{HTML}{EEF2F6}       % pale band behind the contents map and the front box
\\definecolor{codeframe}{HTML}{AEB8C2}
\\definecolor{keyword}{HTML}{10357A}
\\definecolor{comment}{HTML}{4B5A44}
\\definecolor{string}{HTML}{9B3B2E}
\\definecolor{blurb}{HTML}{3B4652}

% ---------------------------------------------------------------------------------------------
% NAVIGATION. The section number has to reach the page HEADER and the page FOOTER, and both of
% those are typeset inside the output routine, long after the section itself was read. Marks are
% the only mechanism that carries a value there correctly, so the mark holds BOTH the number and
% the name as arguments to \\secpair, and each consumer redefines \\secpair to render its half.
\\newcommand{\\secpair}[2]{#2}
\\makeatletter
% Section 0 is the sentinel the index uses: it has a name but no number, so it gets no chip.
\\newcommand{\\hdrsec}{\\def\\secpair##1##2{%
  \\ifnum##1>0\\relax
    {\\setlength{\\fboxsep}{1.6pt}\\colorbox{ink}{\\color{white}\\bfseries##1}}\\hspace{0.5em}%
  \\fi
  \\bfseries##2}}
\\newcount\\tab@i
% THE SECTION STRIP: one cell per section along the bottom edge, the current one filled and tall.
% Fan the printed block and the filled cell walks left to right, so you land in the right section
% before you have read a single word.
\\newcommand{\\tabstrip}[1]{%
  \\begingroup
  \\tab@i=1
  \\@whilenum\\tab@i<${NSEC + 1}\\do{%
    \\ifnum\\tab@i=#1
      \\textcolor{accent}{\\rule[-0.6pt]{0.30cm}{4pt}}%
    \\else
      \\textcolor{rule}{\\rule[-0.6pt]{0.30cm}{1.2pt}}%
    \\fi
    \\hspace{1.3pt}%
    \\advance\\tab@i by 1
  }%
  \\endgroup}
\\newcommand{\\ftrsec}{\\def\\secpair##1##2{\\tabstrip{##1}}}
\\makeatother

\\pagestyle{fancy}
\\fancyhf{}
\\fancyhead[L]{\\small\\sffamily\\color{ink}\\hdrsec\\leftmark}
\\fancyhead[R]{\\small\\sffamily\\color{blurb}\\rightmark}
\\fancyfoot[L]{\\ftrsec\\leftmark}
\\fancyfoot[R]{\\small\\sffamily\\bfseries\\color{ink}\\thepage}
\\renewcommand{\\headrulewidth}{0.4pt}
\\renewcommand{\\sectionmark}[1]{\\markboth{\\protect\\secpair{\\thesection}{#1}}{}}
\\renewcommand{\\subsectionmark}[1]{\\markright{#1}}
\\renewcommand{\\headrule}{\\color{rule}\\hrule height 0.4pt}
\\fancypagestyle{front}{\\fancyhf{}\\fancyfoot[R]{\\small\\sffamily\\bfseries\\color{ink}\\thepage}%
  \\renewcommand{\\headrulewidth}{0pt}}

\\lstset{
    language=C++,
    basicstyle=\\ttfamily\\fontsize{7.6}{8.8}\\selectfont,
    keywordstyle=\\color{keyword}\\bfseries,
    commentstyle=\\color{comment},   % Inconsolata has no true italic; colour alone separates comments
    stringstyle=\\color{string},
    frame=leftline,
    framerule=0.5pt,
    rulecolor=\\color{codeframe},
    breaklines=true,
    breakatwhitespace=true,
    breakindent=1.2em,
    postbreak=\\mbox{\\textcolor{accent}{$\\hookrightarrow$}\\space},
    tabsize=2,
    showstringspaces=false,
    numbers=none,
    xleftmargin=0.35em,
    framexleftmargin=0.35em,
    columns=fullflexible,
    keepspaces=true,
    aboveskip=0.3em,
    belowskip=0.3em,
    extendedchars=false,
}

% ---------------------------------------------------------------------------------------------
% HEADINGS. A section is a solid ink bar with its number set off by a light rule - the landmark
% you thumb for. A subsection is an accent number over a hairline. A template title carries a
% small accent tick, because that is the line your eye actually scans for under contest pressure.
\\newcommand{\\sectionbar}[1]{%
  {\\setlength{\\fboxsep}{0pt}%
   \\colorbox{ink}{\\parbox{\\linewidth}{\\raggedright\\sffamily\\color{white}%
     \\vspace{3.2pt}%
     \\makebox[1.85em][c]{\\large\\bfseries\\thesection}%
     \\textcolor{accentlt}{\\rule[-1.6pt]{0.9pt}{12pt}}\\hspace{0.6em}%
     {\\large\\bfseries #1}%
     \\vspace{3.2pt}}}}}
\\newcommand{\\sectionbarplain}[1]{%
  {\\setlength{\\fboxsep}{0pt}%
   \\colorbox{ink}{\\parbox{\\linewidth}{\\raggedright\\sffamily\\color{white}%
     \\vspace{3.2pt}\\hspace{0.6em}{\\large\\bfseries #1}\\vspace{3.2pt}}}}}
\\titleformat{\\section}[block]{\\normalfont}{}{0pt}{\\sectionbar}
\\titleformat{name=\\section,numberless}[block]{\\normalfont}{}{0pt}{\\sectionbarplain}
\\titleformat{\\subsection}{\\normalsize\\bfseries\\sffamily\\color{ink}}
  {\\color{accent}\\thesubsection}{0.4em}{}
  [\\vspace{-0.72\\baselineskip}{\\color{accent}\\rule{\\linewidth}{0.7pt}}]
\\titleformat{\\subsubsection}{\\small\\bfseries\\sffamily\\color{ink}}
  {\\textcolor{accent}{\\rule[0.4pt]{2.2pt}{6.4pt}}\\hspace{0.4em}\\color{accent}\\thesubsubsection}{0.32em}{}

\\titlespacing*{\\section}{0pt}{0.6em}{0.4em}
\\titlespacing*{\\subsection}{0pt}{0.9em}{0.25em}
\\titlespacing*{\\subsubsection}{0pt}{0.75em}{0.15em}

% A tinted band, used for the per-section topic map.
\\newcommand{\\topicmap}[1]{%
  \\nopagebreak{\\setlength{\\fboxsep}{3.5pt}%
   \\noindent\\colorbox{tint}{\\parbox{\\dimexpr\\linewidth-7pt\\relax}{%
     \\footnotesize\\sffamily\\raggedright\\color{blurb}#1}}\\par}\\nopagebreak}

\\setlength{\\columnseprule}{0.3pt}
\\setlength{\\columnsep}{0.55cm}
\\linespread{1.0}

\\raggedbottom
\\begin{document}

% NOTE: no square brackets anywhere inside \\twocolumn[...] - its optional argument is delimited
% by the first ] at brace level 0, so a \\\\[1em] here would silently truncate the whole block.
\\twocolumn[
  \\begin{center}
    {\\color{rule}\\rule{\\linewidth}{0.7pt}}\\par\\vspace{0.5em}
    {\\sffamily\\footnotesize\\bfseries\\color{accent}%
      I\\,C\\,P\\,C\\ \\ T\\,E\\,A\\,M\\ \\ R\\,E\\,F\\,E\\,R\\,E\\,N\\,C\\,E}\\par\\vspace{0.55em}
    {\\sffamily\\fontsize{40}{44}\\selectfont\\bfseries\\color{ink}${TITLE}}\\par\\vspace{0.5em}
    {\\sffamily\\large\\color{ink}${AUTHOR}\\ \\textcolor{accent}{(${HANDLE})}}\\par\\vspace{0.6em}
    {\\color{accent}\\rule{\\linewidth}{2pt}}\\par\\vspace{0.5em}
    {\\sffamily\\footnotesize\\color{blurb}%
      ${STATS.files} templates \\textperiodcentered\\ ${NSEC} sections \\textperiodcentered\\ ${groupDigits(STATS.lines)} lines \\textperiodcentered\\ \\today}
\\thispagestyle{front}
  \\end{center}
  \\vspace{0.9em}
]

\\noindent{\\setlength{\\fboxsep}{5pt}%
\\fcolorbox{accent}{tint}{\\parbox{\\dimexpr\\linewidth-11.6pt\\relax}{\\footnotesize\\sffamily
{\\bfseries\\color{ink}HOW TO FIND SOMETHING (in order of speed).}
{\\bfseries\\color{accent}1.} You know the routine's name $\\rightarrow$ the alphabetical \\emph{Index of every
routine} at the very back gives the page.
{\\bfseries\\color{accent}2.} You know the topic $\\rightarrow$ the running head on every page names the section
(left) and the sub-topic (right), and the strip along the bottom edge shows which of the
${NSEC} sections you are in; thumb until it matches.
{\\bfseries\\color{accent}3.} You know neither $\\rightarrow$ section \\textbf{12 Problem Solving} maps a problem's
\\emph{shape} and its \\emph{constraints} onto the technique, and \\texttt{docs/PLAYBOOK.md} does the
same at more length.
\\par\\vspace{0.45em}
{\\bfseries\\color{ink}BEFORE YOU PASTE.} Line 1 of every file says what it does; a \\texttt{// Needs:} line names
the templates it depends on; a \\texttt{// NOTE:} line warns when another file declares the same
symbol. Vertices and array positions are \\textbf{0-based} unless the header says otherwise.
Geometry: \\texttt{P\\textless ll\\textgreater} for every decision, \\texttt{P\\textless ld\\textgreater}
only when the answer is a real number.
}}}
\\vspace{0.9em}

{\\footnotesize\\sffamily
\\setcounter{tocdepth}{2}
\\makeatletter
% tight, two-level contents: the section flush left in bold ink, its sub-topics dotted underneath
\\renewcommand*\\l@section[2]{%
  \\addpenalty{-\\@highpenalty}\\vskip 1.15ex \\@plus\\p@
  \\begingroup
  % \\numberline sizes its number box from \\@tempdima - leave it unset and the section NAME is
  % pushed clean off the column by whatever width happened to be in there.
  \\setlength\\@tempdima{1.6em}%
  \\parindent\\z@ \\rightskip\\@pnumwidth \\parfillskip-\\@pnumwidth
  \\leavevmode\\bfseries\\color{ink}#1\\nobreak\\hfil\\nobreak
  \\hb@xt@\\@pnumwidth{\\hss\\color{accent}#2}\\par
  \\endgroup}
\\renewcommand*\\l@subsection{\\@dottedtocline{2}{1.2em}{2.4em}}
\\makeatother
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

let secNo = 0;
for (const section of sections) {
    const sectionName = cleanName(section.name);
    secNo++;
    // Never let a section bar land alone at the foot of a column - the bar IS the landmark you
    // thumb for, so it has to sit above something.
    latex += `\\needspace{12\\baselineskip}\n`;
    latex += `\\section{${escapeLatex(sectionName)}}\n`;

    const entries = getSortedEntries(section.fullPath);

    // A one-paragraph map of the section with page numbers: the fastest way to find a topic
    // once you know roughly where it lives.
    const topics = entries
        .filter(e => e.isDir || e.name.endsWith('.cpp'))
        .map(e => cleanName(e.isDir ? e.name : path.basename(e.name, '.cpp')));
    if (topics.length > 1) {
        const links = topics.map((t, i) =>
            `${escapeLatex(t)}~\\pageref{sec:${secNo}:${i}}`).join(' \\textperiodcentered\\ ');
        latex += `\\topicmap{${links}}\n`;
    }
    latex += `\n`;
    let topicNo = -1;

    // Process entries (files and subdirectories) together in sorted order
    for (const entry of entries) {
        if (entry.isDir) {
            const subdirName = cleanName(entry.name);
            topicNo++;
            latex += `\\subsection{${escapeLatex(subdirName)}}\\label{sec:${secNo}:${topicNo}}\n\n`;

            const subFiles = getSortedEntries(entry.fullPath).filter(e => !e.isDir && e.name.endsWith('.cpp'));
            for (const file of subFiles) {
                const fileName = cleanName(path.basename(file.name, '.cpp'));
                const code = fs.readFileSync(file.fullPath, 'utf-8').trimEnd();
                latex += `\\needspace{6\\baselineskip}\n`;
                latex += `\\subsubsection{${escapeLatex(fileName)}}${indexEntries(code)}\n`;
                latex += blurbLine(code);
                latex += `\\begin{lstlisting}\n${trimBlurbLine(code)}\n\\end{lstlisting}\n\n`;
            }
        } else if (entry.name.endsWith('.cpp')) {
            const fileName = cleanName(path.basename(entry.name, '.cpp'));
            const code = fs.readFileSync(entry.fullPath, 'utf-8').trimEnd();
            latex += `\\needspace{6\\baselineskip}\n`;
            topicNo++;
            latex += `\\subsection{${escapeLatex(fileName)}}\\label{sec:${secNo}:${topicNo}}${indexEntries(code)}\n`;
            latex += blurbLine(code);
            latex += `\\begin{lstlisting}\n${trimBlurbLine(code)}\n\\end{lstlisting}\n\n`;
        }
    }
}

// The index is the fastest lookup in the book, so it gets the whole page width and THREE columns
// instead of the class default of two - denser to scan, and it saves pages.
latex += `\\makeatletter
\\renewcommand{\\indexname}{Index of every routine (alphabetical)}
% One accent letter per initial, emitted by makeindex when it is run with notebook.ist.
\\newcommand{\\indexgroup}[1]{%
  \\par\\vspace{0.55em}\\noindent{\\sffamily\\bfseries\\normalsize\\color{accent}#1}%
  \\par\\vspace{0.12em}\\nopagebreak}
\\renewenvironment{theindex}{%
  \\clearpage\\onecolumn
  \\phantomsection\\addcontentsline{toc}{section}{Index of every routine}%
  \\section*{\\indexname}%
  \\markboth{\\protect\\secpair{0}{Index of every routine}}{}%
  \\vspace{0.3em}%
  \\parindent\\z@ \\parskip\\z@ \\@plus.3\\p@\\relax
  \\let\\item\\@idxitem \\raggedright\\footnotesize
  \\begin{multicols}{3}}%
  {\\end{multicols}}
\\makeatother
\\printindex
`;
latex += `\\end{document}\n`;

fs.writeFileSync(outputFile, latex, 'utf-8');
console.log('Generated: ' + outputFile);

// Generate toc.yaml
function generateTocYaml() {
    const yamlOutputFile = path.join(__dirname, 'toc.yaml');
    let yamlLines = [];
    yamlLines.push("# Baskot Temp - Table of Contents");
    yamlLines.push("title: Baskot Temp");
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

// THREE passes with makeindex in the middle, and that count is not padding:
//   1. writes .aux / .toc / .idx      2. after makeindex, sets the contents and the index
//   3. settles the \pageref cross-references in every per-section topic map
// Skip the whole step with:  node export_notebook.js --no-pdf
const baseName = path.basename(outputFile, '.tex');
if (process.argv.includes('--no-pdf')) {
    console.log('Skipped PDF (--no-pdf).');
} else try {
    for (const ext of ['.aux', '.toc', '.idx', '.ind', '.out']) {
        const f = path.join(texDir, baseName + ext);
        if (fs.existsSync(f)) fs.unlinkSync(f);
    }
    console.log('Compiling PDF (pass 1/3)...');
    execSync(`${pdflatex} -interaction=nonstopmode -halt-on-error "${texFile}"`, { cwd: texDir, stdio: 'pipe' });
    console.log('Building the index...');
    try {
        const ist = fs.existsSync(path.join(texDir, baseName + '.ist')) ? `-s "${baseName}.ist" ` : '';
        execSync(`makeindex ${ist}-q "${baseName}.idx"`, { cwd: texDir, stdio: 'pipe' });
    } catch {
        console.warn('WARNING: makeindex failed - the PDF will be built WITHOUT its index.');
    }
    console.log('Compiling PDF (pass 2/3)...');
    execSync(`${pdflatex} -interaction=nonstopmode -halt-on-error "${texFile}"`, { cwd: texDir, stdio: 'pipe' });
    console.log('Compiling PDF (pass 3/3)...');
    execSync(`${pdflatex} -interaction=nonstopmode -halt-on-error "${texFile}"`, { cwd: texDir, stdio: 'pipe' });

    const log = fs.readFileSync(path.join(texDir, baseName + '.log'), 'utf-8');
    const pages = (log.match(/Output written on .*?\((\d+) pages/) || [])[1];
    console.log('Generated: ' + outputFile.replace('.tex', '.pdf') + (pages ? ` (${pages} pages)` : ''));

    for (const ext of ['.aux', '.log', '.toc', '.out', '.idx', '.ilg', '.ind']) {
        const f = path.join(texDir, baseName + ext);
        if (fs.existsSync(f)) fs.unlinkSync(f);
    }
    console.log('Cleaned up auxiliary files.');
} catch (err) {
    console.error('PDF compilation failed. Make sure pdflatex is installed and in PATH.');
    console.error(err.stderr?.toString() || err.message);
    process.exit(1);
}
