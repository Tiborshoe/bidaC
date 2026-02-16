<?php
// Run with: php -S localhost:8000

// ---------------------------------------------------------
// HANDLE AJAX REQUESTS
// ---------------------------------------------------------
if (isset($_POST['action'])) {
    $baseDir = __DIR__;
    
    // --- COMPILE ACTION ---
    if ($_POST['action'] === 'compile') {
        $code = isset($_POST['code']) ? $_POST['code'] : '';

        // 1. Save input to file
        file_put_contents($baseDir . '/input.bis', $code);

        // 2. Prepare paths
        $compilerPath = $baseDir . '/bisdac';
        $inputFile    = $baseDir . '/input.bis';
        
        // Windows support check
        if (strtoupper(substr(PHP_OS, 0, 3)) === 'WIN') {
            $compilerPath .= ".exe";
            $cmd = "\"$compilerPath\" < \"$inputFile\" 2>&1";
        } else {
            $cmd = "$compilerPath < $inputFile 2>&1";
        }

        // 3. Execute Compiler
        $output = shell_exec($cmd);
        
        // Parse output
        $sections = parseCompilerOutput($output);

        // 4. Return Data
        echo json_encode([
            'ok' => true,
            'output'   => $sections['output'],
            'assembly' => $sections['assembly'],
            'binary'   => $sections['binary']
        ]);
        exit;
    }
}

function parseCompilerOutput($output) {
    $result = [
        'output' => '',
        'assembly' => '',
        'binary' => ''
    ];
    
    if (empty($output)) {
        return $result;
    }
    
    $lines = explode("\n", $output);
    $currentSection = 'output';
    $outputBuffer = [];
    $assemblyBuffer = [];
    $binaryBuffer = [];
    
    foreach ($lines as $line) {
        $trimmed = trim($line);
        if (strpos($line, 'Assembly:') === 0) {
            $currentSection = 'assembly';
        } elseif (strpos($line, 'Binary:') === 0 || strpos($line, 'Hex:') === 0) {
            $currentSection = 'binary';
        }
        
        if ($currentSection === 'assembly') {
            $assemblyBuffer[] = $line;
        } elseif ($currentSection === 'binary') {
            $binaryBuffer[] = $line;
        } else {
            $outputBuffer[] = $line;
        }
    }
    
    $result['output'] = implode("\n", $outputBuffer);
    $result['assembly'] = implode("\n", $assemblyBuffer);
    $result['binary'] = implode("\n", $binaryBuffer);
    
    return $result;
}
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>BISDAC IDE</title>
    <style>
        :root {
            /* Cyberpunk Neon Palette */
            --bg-dark: #0b1120;       /* Very dark navy */
            --panel-bg: #151b2e;      /* Slightly lighter panel bg */
            --border-glow: #2d6cd8;   /* Bright neon blue border */
            --text-main: #e2e8f0;     /* White/Grey text */
            --text-dim: #94a3b8;      /* Dimmed text */
            
            /* Accent Colors */
            --btn-run: #f97316;       /* Orange */
            --btn-restart: #22c55e;   /* Green */
            --btn-help: #3b82f6;      /* Blue */
            --btn-open: #a855f7;      /* Purple */
        }

        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: 'Segoe UI', 'Roboto', sans-serif;
            background-color: var(--bg-dark);
            background-image: 
                linear-gradient(rgba(45, 108, 216, 0.05) 1px, transparent 1px),
                linear-gradient(90deg, rgba(45, 108, 216, 0.05) 1px, transparent 1px);
            background-size: 30px 30px;
            color: var(--text-main);
            height: 100vh;
            overflow: hidden;
            padding: 20px;
            display: flex;
            flex-direction: column;
            gap: 20px;
        }

        /* --- HEADER SECTION --- */
        .header {
            display: flex;
            justify-content: space-between;
            align-items: center;
            height: 60px;
            padding: 0 10px;
        }

        /* UPDATED LOGO STYLING */
        .logo-img {
            height: 150px; /* Reduced slightly to fit better within 60px header */
            width: auto;
            filter: drop-shadow(0 0 8px rgba(96, 165, 250, 0.4));
            transition: transform 0.3s ease;
            vertical-align: middle;
        }

        .logo-img:hover {
            transform: scale(1.05);
            filter: drop-shadow(0 0 12px rgba(96, 165, 250, 0.8));
        }

        .header-actions {
            display: flex;
            gap: 15px;
        }

        .btn {
            border: none;
            padding: 10px 24px;
            border-radius: 8px;
            font-weight: 700;
            font-size: 14px;
            color: white;
            cursor: pointer;
            transition: all 0.2s ease;
            text-transform: uppercase;
            letter-spacing: 1px;
            display: flex;
            align-items: center;
            gap: 8px;
            box-shadow: 0 4px 6px rgba(0,0,0,0.3);
        }

        .btn:hover {
            transform: translateY(-2px);
            filter: brightness(1.1);
        }

        .btn:active {
            transform: translateY(0);
        }

        .btn-open { background: var(--btn-open); box-shadow: 0 0 10px rgba(168, 85, 247, 0.4); }
        .btn-run { background: var(--btn-run); box-shadow: 0 0 10px rgba(249, 115, 22, 0.4); }
        .btn-restart { background: var(--btn-restart); box-shadow: 0 0 10px rgba(34, 197, 94, 0.4); }
        .btn-help { background: var(--btn-help); box-shadow: 0 0 10px rgba(59, 130, 246, 0.4); }

        /* --- MAIN LAYOUT (GRID) --- */
        .main-container {
            display: grid;
            grid-template-columns: 1.3fr 1fr;
            grid-template-rows: 1fr 1fr;
            grid-template-areas: 
                "editor output"
                "editor details";
            gap: 20px;
            flex: 1;
            min-height: 0;
        }

        /* --- PANELS --- */
        .panel {
            background: var(--panel-bg);
            border: 2px solid var(--border-glow);
            border-radius: 16px;
            box-shadow: 0 0 20px rgba(45, 108, 216, 0.15);
            display: flex;
            flex-direction: column;
            overflow: hidden;
            position: relative;
        }

        .panel-header {
            padding: 12px 20px;
            background: rgba(45, 108, 216, 0.1);
            border-bottom: 1px solid rgba(45, 108, 216, 0.3);
            font-size: 14px;
            font-weight: 600;
            color: #60a5fa;
            letter-spacing: 1px;
            text-transform: uppercase;
            display: flex;
            align-items: center;
            gap: 10px;
        }

        .panel-content {
            flex: 1;
            position: relative;
            overflow: hidden;
        }

        /* --- EDITOR AREA --- */
        .area-editor { grid-area: editor; }

        .editor-wrapper {
            display: flex;
            height: 100%;
            font-family: 'Consolas', 'Monaco', monospace;
            font-size: 15px;
        }

        .line-numbers {
            background: #0f1522;
            color: #4b5563;
            padding: 15px 10px;
            text-align: right;
            border-right: 1px solid #1e293b;
            user-select: none;
            min-width: 45px;
            line-height: 1.6;
        }

        .code-editor {
            flex: 1;
            background: transparent;
            color: #e2e8f0;
            border: none;
            outline: none;
            padding: 15px;
            white-space: pre;
            overflow: auto;
            line-height: 1.6;
            caret-color: var(--btn-run);
        }

        /* --- OUTPUT AREA --- */
        .area-output { grid-area: output; }

        .terminal-output {
            padding: 15px;
            font-family: 'Consolas', monospace;
            font-size: 14px;
            color: #e2e8f0;
            height: 100%;
            overflow: auto;
            white-space: pre-wrap;
        }
        
        /* --- DETAILS AREA --- */
        .area-details {
            grid-area: details;
            display: flex;
            flex-direction: column;
        }
        
        .details-content {
            padding: 15px;
            font-family: 'Consolas', monospace;
            font-size: 13px;
            color: #94a3b8;
            height: 100%;
            overflow: auto;
            white-space: pre;
            line-height: 1.4;
        }

        /* --- SCROLLBARS --- */
        ::-webkit-scrollbar { width: 8px; height: 8px; }
        ::-webkit-scrollbar-track { background: #0b1120; }
        ::-webkit-scrollbar-thumb { background: #334155; border-radius: 4px; }
        ::-webkit-scrollbar-thumb:hover { background: #475569; }

        /* --- MODAL --- */
        .modal {
            display: none;
            position: fixed;
            top: 0; left: 0; width: 100%; height: 100%;
            background: rgba(0,0,0,0.8);
            backdrop-filter: blur(5px);
            z-index: 999;
            justify-content: center;
            align-items: center;
        }
        .modal-content {
            background: var(--panel-bg);
            border: 2px solid var(--border-glow);
            width: 800px; /* Wider for full docs */
            max-height: 85vh; /* Prevent overflow */
            border-radius: 16px;
            padding: 0; /* Removing padding from container to handle scrolling better */
            color: white;
            box-shadow: 0 0 50px rgba(45, 108, 216, 0.4);
            display: flex;
            flex-direction: column;
        }
        .modal-header {
            padding: 20px 30px;
            border-bottom: 1px solid #334155;
            display: flex;
            justify-content: space-between;
            align-items: center;
        }
        .modal-header h2 { color: var(--btn-help); margin: 0; }
        .modal-body {
            padding: 30px;
            overflow-y: auto;
            line-height: 1.6;
        }
        .modal-body h3 {
            color: var(--btn-run);
            margin-top: 25px;
            margin-bottom: 10px;
            border-bottom: 1px solid #334155;
            padding-bottom: 5px;
        }
        .modal-body code {
            background: #0f1522;
            padding: 2px 6px;
            border-radius: 4px;
            font-family: 'Consolas', monospace;
            color: #a855f7;
        }
        .modal-body pre {
            background: #0f1522;
            color: #e2e8f0;
            padding: 15px;
            border-radius: 8px;
            overflow-x: auto;
            margin: 10px 0;
            border: 1px solid #334155;
        }
        .modal-body ul {
            margin-left: 20px;
            margin-bottom: 15px;
        }

        /* --- RESPONSIVE --- */
        @media (max-width: 1024px) {
            .main-container {
                grid-template-columns: 1fr;
                grid-template-rows: 1.5fr 1fr 1fr;
                grid-template-areas: "editor" "output" "details";
            }
            body { overflow: auto; height: auto; }
            .modal-content { width: 95%; margin: 20px; }
        }
    </style>
</head>
<body>

    <header class="header">
        <div class="logo-container">
            <img src="Code-Photoroom.png" alt="Bisdac Logo" class="logo-img">
        </div>

        <div class="header-actions">
            <input type="file" id="fileInput" accept=".bis,.txt" style="display: none;">
            
            <button class="btn btn-run" id="runBtn">
                <span>▶</span> Run
            </button>
            <button class="btn btn-restart" id="restartBtn">
                <span>↻</span> Restart
            </button>
             <button class="btn btn-open" id="openBtn">
                <span>📂</span> Open
            </button>
            <button class="btn btn-help" id="helpBtn">
                <span>?</span> Help
            </button>
        </div>
    </header>

    <div class="main-container">
        
        <div class="panel area-editor">
            <div class="panel-header">
                <span>Code Editor</span>
            </div>
            <div class="panel-content">
                <div class="editor-wrapper">
                    <div class="line-numbers" id="lineNumbers">1</div>
                    <div class="code-editor" id="codeEditor" contenteditable="true" spellcheck="false">litir name = "World";
numero x = 100;
numero y = 100;
numero a = x + x - y;
ipagawas "Hello " name;
ipagawas "Value of a: " a;</div>
                </div>
            </div>
        </div>

        <div class="panel area-output">
            <div class="panel-header">
                <span>Output</span>
            </div>
            <div class="panel-content">
                <div class="terminal-output" id="programOutput">
                    <span style="opacity: 0.5;">// Program output will appear here...</span>
                </div>
            </div>
        </div>

        <div class="panel area-details">
            <div class="panel-header">
                <span>Assembly / Binary / Hex</span>
            </div>
            <div class="panel-content">
                <div class="details-content" id="detailsOutput">
                    <span style="opacity: 0.5;">// Technical details will appear here...</span>
                </div>
            </div>
        </div>

    </div>

    <div class="modal" id="helpModal">
        <div class="modal-content">
            <div class="modal-header">
                <h2>Bisdac Language Documentation</h2>
                <span id="closeModal" style="cursor:pointer; font-size:24px;">&times;</span>
            </div>
            <div class="modal-body">
                <h3>What is Bisdac?</h3>
                <p>Bisdac is a Bisaya-inspired programming language designed for educational purposes. It compiles to MIPS64 assembly and demonstrates compiler construction principles.</p>

                <h3>Data Types</h3>
                <ul>
                    <li><code>numero</code> - Integer variables (like <code>int</code>)</li>
                    <li><code>litir</code> - String and character variables</li>
                </ul>
                <pre>numero x = 10;
litir msg = "Kumusta!";
litir ch = 'A';</pre>

                <h3>Variable Declaration</h3>
                <pre>numero a = 50;        <# Integer #>
numero b;             <# Default value 0 #>
litir name = "Juan";  <# String #>
litir grade = 'A';    <# Character #></pre>

                <h3>Arithmetic Operations</h3>
                <p>Supports: <code>+</code> <code>-</code> <code>*</code> <code>/</code> and parentheses <code>( )</code></p>
                <pre>numero result = (10 + 5) * 2;
numero calc = a - b / 4;</pre>

                <h3>Output Statement</h3>
                <p>Use <code>ipagawas</code> to display values:</p>
                <pre>ipagawas "Hello World!";
ipagawas x;
ipagawas "Score: " x;</pre>

                <h3>Comments</h3>
                <pre>&lt;# This is a comment #&gt;</pre>

                <h3>Important Rules</h3>
                <ul>
                    <li>Every statement must end with semicolon <code>;</code></li>
                    <li>String literals use double quotes <code>"text"</code></li>
                    <li>Character literals use single quotes <code>'A'</code></li>
                    <li>Variables must be declared before use</li>
                </ul>

                <h3>Example Program</h3>
                <pre>&lt;# Calculate area #&gt;
numero width = 10;
numero height = 5;
numero area = width * height;

ipagawas "Area is: " area;</pre>
            </div>
        </div>
    </div>

    <script>
        // --- 1. Line Numbers Logic ---
        const editor = document.getElementById('codeEditor');
        const lineNumbers = document.getElementById('lineNumbers');

        function updateLineNumbers() {
            const lines = editor.innerText.split('\n').length;
            lineNumbers.innerHTML = Array.from({length: lines}, (_, i) => i + 1).join('<br>');
        }

        editor.addEventListener('input', updateLineNumbers);
        editor.addEventListener('keyup', updateLineNumbers);
        updateLineNumbers(); // Init

        // --- 2. Button Logic ---
        document.getElementById('runBtn').addEventListener('click', async function() {
            const btn = this;
            btn.innerHTML = '<span>⏳</span> ...';
            
            const code = editor.innerText;
            const outputDiv = document.getElementById('programOutput');
            const detailsDiv = document.getElementById('detailsOutput');

            outputDiv.innerText = 'Running...';
            detailsDiv.innerText = 'Compiling...';

            try {
                const formData = new FormData();
                formData.append('action', 'compile');
                formData.append('code', code);

                const response = await fetch('', { method: 'POST', body: formData });
                const result = await response.json();

                if (result.ok) {
                    outputDiv.innerText = result.output || 'No output';
                    // Combine Assembly and Binary for the "Details" panel
                    detailsDiv.innerHTML = 
                        "<strong style='color:#f97316'>[ASSEMBLY]</strong>\n" + (result.assembly || "None") + 
                        "\n\n<strong style='color:#22c55e'>[BINARY / HEX]</strong>\n" + (result.binary || "None");
                } else {
                    outputDiv.innerText = "Compilation Failed.";
                }
            } catch (error) {
                outputDiv.innerText = 'Error: ' + error.message;
            } finally {
                btn.innerHTML = '<span>▶</span> Run';
            }
        });

        // Restart
        document.getElementById('restartBtn').addEventListener('click', () => {
            if(confirm("Clear everything?")) {
                editor.innerText = '';
                document.getElementById('programOutput').innerText = '// Program output will appear here...';
                document.getElementById('detailsOutput').innerText = '// Technical details will appear here...';
                updateLineNumbers();
            }
        });

        // Open File
        const fileInput = document.getElementById('fileInput');
        document.getElementById('openBtn').addEventListener('click', () => fileInput.click());
        fileInput.addEventListener('change', (e) => {
            const file = e.target.files[0];
            if (!file) return;
            const reader = new FileReader();
            reader.onload = (e) => {
                editor.innerText = e.target.result;
                updateLineNumbers();
            };
            reader.readAsText(file);
            fileInput.value = '';
        });

        // Modal
        const modal = document.getElementById('helpModal');
        document.getElementById('helpBtn').addEventListener('click', () => modal.style.display = 'flex');
        document.getElementById('closeModal').addEventListener('click', () => modal.style.display = 'none');
        window.addEventListener('click', (e) => {
            if (e.target === modal) modal.style.display = 'none';
        });
    </script>
</body>
</html>