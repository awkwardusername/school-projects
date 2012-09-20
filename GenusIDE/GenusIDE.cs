#region Using Directives

using System;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using GenusIDE.Properties;
using ScintillaNET;

#endregion Using Directives

namespace GenusIDE {
    internal sealed partial class GenusIDE : Form {

        #region Constants

        private const string NEW_DOCUMENT_TEXT = "Untitled";
        private const int LINE_NUMBERS_MARGIN_WIDTH = 35;

        #endregion Constants

        #region Fields

        private int _newDocumentCount = 0;
        private string[] _args;
        private int _zoomLevel;
        private About _splash;
        private string _filetype = "txt";
        private string filename = "";

        #endregion Fields

        #region Methods

        private DocumentForm NewDocument() {
            DocumentForm doc = new DocumentForm();
            SetScintillaToCurrentOptions(doc);
            doc.Text = String.Format(CultureInfo.CurrentCulture, "{0}{1}", NEW_DOCUMENT_TEXT, ++_newDocumentCount);

            setStatusMessages(doc.Text);

            doc.Show(dockPanel);
            // TODO Implement 
            //toolIncremental.Searcher.Scintilla = doc.Scintilla;
            return doc;
        }

        private DocumentForm NewDocument(string ext) {
            DocumentForm doc = new DocumentForm();
            //SetScintillaToCurrentOptions(doc);
            doc.Text = String.Format(CultureInfo.CurrentCulture, "{0}{1}." + ext, NEW_DOCUMENT_TEXT, ++_newDocumentCount);

            setStatusMessages(doc.Text);

            doc.Show(dockPanel);
            
            return doc;
        }

        private DocumentForm OpenFile(string filePath) {
            DocumentForm doc = new DocumentForm();

            SetScintillaToCurrentOptions(doc);

            doc.Scintilla.Text = File.ReadAllText(filePath);
            doc.Scintilla.UndoRedo.EmptyUndoBuffer();
            doc.Scintilla.Modified = false;
            doc.Text = Path.GetFileName(filePath);
            doc.FilePath = filePath;

            setStatusMessages(doc.Text);

            doc.Show(dockPanel);
           

            return doc;
        }

        private void SetScintillaToCurrentOptions(DocumentForm doc) {
            // Turn on line numbers?
            if (lineNumbersToolStripMenuItem.Checked)
                doc.Scintilla.Margins.Margin0.Width = LINE_NUMBERS_MARGIN_WIDTH;
            else
                doc.Scintilla.Margins.Margin0.Width = 0;

            // Turn on white space?
            if (whitespaceSymbolsToolStripMenuItem.Checked)
                doc.Scintilla.Whitespace.Mode = WhitespaceMode.VisibleAlways;
            else
                doc.Scintilla.Whitespace.Mode = WhitespaceMode.Invisible;

            // Turn on word wrap?
            if (wordWrapToolStripMenuItem.Checked)
                doc.Scintilla.LineWrapping.Mode = LineWrappingMode.Word;
            else
                doc.Scintilla.LineWrapping.Mode = LineWrappingMode.None;

            // Show EOL?
            //doc.Scintilla.EndOfLine.IsVisible = endOfLineToolStripMenuItem.Checked;

            // Set the zoom
            doc.Scintilla.Zoom = _zoomLevel;
        }

        private void CToolStripMenuItemClick(object sender, EventArgs e) {
            SetLanguage("cpp");
        }

        private void OpenToolStripMenuItemClick(object sender, EventArgs e) {
            FileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Title = "Open file...";
            openFileDialog.Filter = "C/C++ (*.c, *.cpp, *.cxx, *.h, *.hxx)|*.c;*.cpp;*.cxx;*.h;*.hxx|C# (*.cs)|*.cs|HTML (*.html, *.htm)|*.html;*.htm|Text (*.txt)|*.txt|All Files|*.*";

            if (openFileDialog.ShowDialog() != DialogResult.OK)
                return;

            foreach (string filePath in openFileDialog.FileNames) {
                // Ensure this file isn't already open
                bool isOpen = false;
                foreach (DocumentForm documentForm in dockPanel.Documents) {
                    if (filePath.Equals(documentForm.FilePath, StringComparison.OrdinalIgnoreCase)) {
                        documentForm.Select();
                        isOpen = true;
                        break;
                    }
                }

                // Open the files
                if (!isOpen)
                    OpenFile(filePath);
            }

            _filetype = openFileDialog.FileName.Split('.').Last();

            if (_filetype.Equals("c") || _filetype.Equals("cpp") || _filetype.Equals("h") || _filetype.Equals("cxx")) {
                SetLanguage("cpp");
            } else 
                SetLanguage(_filetype);
            
            setStatusMessages(openFileDialog.FileName.Split('\\').Last());
        }

        private void setStatusMessages(string filename) {
            saveToolStripMenuItem.Text = "S&ave " + filename;
            closetoolStripMenuItem3.Text = "&Close" + filename;

            setStatusLabel(filename + " opened.");

            Text = "GenusIDE - " + filename;
        }

        private void ExitToolStripMenuItemClick(object sender, EventArgs e) {
            Dispose();
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e) {
            using (About about = new About()) {
                about.ShowDialog(this);
            }
        } 

        #endregion Methods

        #region Properties

        public DocumentForm ActiveDocument {
            get {
                return dockPanel.ActiveDocument as DocumentForm;
            }
        }

        #endregion Properties

        private void emptyTextFileToolStripMenuItem_Click(object sender, EventArgs e) {
            NewDocument();
            SetLanguage(String.Empty);
        }

        private void dockPanel_ActiveDocumentChanged(object sender, EventArgs e) {
            // Update the main form _text to show the current document
            if (ActiveDocument != null)
                setStatusMessages(ActiveDocument.Text);
            else
                this.Text = Program.Title;
        }

        private void GenusIDE_Load(object sender, EventArgs e) {
            NewDocument();
            zoomtrackBar.Value = _zoomLevel;
            _splash.Close();
            _splash.Dispose();
        }

        /** TODO currently dangerous code, but working.
        private void LoadFonts() {
            var fontList = new List<string>(); 
            foreach (FontFamily ff in System.Drawing.FontFamily.Families) {
                if (ff.IsStyleAvailable(FontStyle.Regular)) {
                    Font font = new Font(ff, 10);
                    LogFont lf = new LogFont();
                    font.ToLogFont(lf);
                    if (lf.lfPitchAndFamily == 1) {
                        fontList.Add(lf.lfFaceName);
                    }
                }
            }

            fontSelectComboBox.ComboBox.DataSource = fontList;
        } **/

        private void SetLanguage(string language) {
            _filetype = language;
            if ("ini".Equals(language, StringComparison.OrdinalIgnoreCase)) {
                // Reset/set all styles and prepare _scintilla for custom lexing

                ActiveDocument.IniLexer = true;
                IniLexer.Init(ActiveDocument.Scintilla);
            } else {
                // Use a built-in lexer and configuration
                ActiveDocument.IniLexer = false;
                ActiveDocument.Scintilla.ConfigurationManager.Language = language;

                // Smart indenting...
                if ("cs".Equals(language, StringComparison.OrdinalIgnoreCase))
                    ActiveDocument.Scintilla.Indentation.SmartIndentType = SmartIndent.CPP;
                else
                    ActiveDocument.Scintilla.Indentation.SmartIndentType = SmartIndent.None;
            }
        }

        public GenusIDE(About splash) {
            _splash = splash;
            InitializeComponent();
            //LoadFonts();          // dangerous. Causes AccessViolationException most of the times. 
            var renderer = new ToolStripProfessionalRenderer();
            renderer.ColorTable.UseSystemColors = true;
            renderer.RoundedEdges = false;
            ToolStripManager.Renderer = renderer;

            // Set the application title
            Text = Program.Title;
            aboutToolStripMenuItem.Text = String.Format(CultureInfo.CurrentCulture, "&About {0}", Program.Title);
        }

       

        private void mOESourceFileToolStripMenuItem_Click(object sender, EventArgs e) {
            NewDocument("moe");
            SetLanguage("ini");
        }

        private void okashiSourceFileToolStripMenuItem_Click(object sender, EventArgs e) {
            NewDocument("oks");

        }

        private void cCSourceFileToolStripMenuItem_Click(object sender, EventArgs e) {
            NewDocument("c");
            SetLanguage("cpp");
        }

        private void cSourceFileToolStripMenuItem_Click(object sender, EventArgs e) {
            NewDocument("cpp");
            SetLanguage("cpp");
        }

        private void cSourceFileToolStripMenuItem1_Click(object sender, EventArgs e) {
            NewDocument("cs");
            SetLanguage("cs");
        }

        private void pHPSourceFileToolStripMenuItem_Click(object sender, EventArgs e) {
            NewDocument("php");
            SetLanguage("php");
        }

        private void hTMLSourceFileToolStripMenuItem_Click(object sender, EventArgs e) {
            NewDocument("html");
            SetLanguage("html");
        }

        private void javaSourceFileToolStripMenuItem_Click(object sender, EventArgs e) {
            NewDocument("java");
            SetLanguage("java");
        }

        private void zoomtrackBar_ValueChanged(object sender, EventArgs e) {
            
        }

        private void whitespaceSymbolsToolStripMenuItem_Click(object sender, EventArgs e) {
            //whiteSpaceSymbolsToolStripMenuItem.C
            // Toggle the whitespace mode for all open files
            whitespaceSymbolsToolStripMenuItem.Checked = !whitespaceSymbolsToolStripMenuItem.Checked;
            foreach (DocumentForm doc in dockPanel.Documents) {
                if (whitespaceSymbolsToolStripMenuItem.Checked)
                    doc.Scintilla.Whitespace.Mode = WhitespaceMode.VisibleAlways;
                else
                    doc.Scintilla.Whitespace.Mode = WhitespaceMode.Invisible;
            }
        }

        private void closetoolStripMenuItem2_Click(object sender, EventArgs e) {
            ActiveDocument.Close();
            setStatusLabel("Document closed.");
        }

        private void saveAllToolStripMenuItem_Click(object sender, EventArgs e) {
            foreach (DocumentForm doc in dockPanel.Documents) {
                doc.Activate();
                doc.Save();
            }
            setStatusLabel("All Files Saved.");
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e) {
            if (ActiveDocument != null) {
                if (ActiveDocument.Save()) {
                    setStatusLabel(ActiveDocument.Text + " saved.");
                }
            }
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e) {
            if (ActiveDocument != null)
                if (ActiveDocument.SaveAs())
                    setStatusLabel(ActiveDocument.Text + " saved.");
        }

        private void printToolStripMenuItem_Click(object sender, EventArgs e) {
            if (ActiveDocument != null) {
                ActiveDocument.Scintilla.Printing.PrintPreview();
                setStatusLabel("Displaying printing options...");
            }
        }

        private void setStatusLabel(string status) {
            statusLabel.Text = status;
        }

        private void UpdateAllScintillaZoom() {
            // Update zoom level for all files
            foreach (DocumentForm doc in dockPanel.Documents)
                doc.Scintilla.Zoom = _zoomLevel;
        }

        private void lineNumbersToolStripMenuItem_Click(object sender, EventArgs e) {
            // Toggle the line numbers margin for all documents
            lineNumbersToolStripMenuItem.Checked = !lineNumbersToolStripMenuItem.Checked;
            foreach (DocumentForm docForm in dockPanel.Documents) {
                if (lineNumbersToolStripMenuItem.Checked)
                    docForm.Scintilla.Margins.Margin0.Width = LINE_NUMBERS_MARGIN_WIDTH;
                else
                    docForm.Scintilla.Margins.Margin0.Width = 0;
            }
        }

        private void wordWrapToolStripMenuItem_Click(object sender, EventArgs e) {
            // Toggle word wrap for all open files
            wordWrapToolStripMenuItem.Checked = !wordWrapToolStripMenuItem.Checked;
            foreach (DocumentForm doc in dockPanel.Documents) {
                if (wordWrapToolStripMenuItem.Checked)
                    doc.Scintilla.LineWrapping.Mode = LineWrappingMode.Word;
                else
                    doc.Scintilla.LineWrapping.Mode = LineWrappingMode.None;
            }
        }

        private void findToolStripMenuItem_Click(object sender, EventArgs e) {
            ActiveDocument.Scintilla.FindReplace.ShowFind();
        }

        private void undotoolStripMenuItem2_Click(object sender, EventArgs e) {
            if (ActiveDocument != null)
                ActiveDocument.Scintilla.UndoRedo.Undo();
        }

        private void redotoolStripMenuItem2_Click(object sender, EventArgs e) {
            if (ActiveDocument != null)
                ActiveDocument.Scintilla.UndoRedo.Redo();
        }

        private void cToolStripMenuItem1_Click(object sender, EventArgs e) {
            SetLanguage("cs");
        }

        private void pHPToolStripMenuItem_Click(object sender, EventArgs e) {
            SetLanguage("php");
        }

        private void hTMLToolStripMenuItem_Click(object sender, EventArgs e) {
            SetLanguage("html");
        }

        private void javaToolStripMenuItem_Click(object sender, EventArgs e) {
            SetLanguage("java");
        }

        private void zoomtrackBar_Scroll(object sender, EventArgs e) {
            _zoomLevel = zoomtrackBar.Value;
            UpdateAllScintillaZoom();
        }

        private void compileToolStripMenuItem_Click(object sender, EventArgs e) {
            Process cmd = new Process();
            cmd.StartInfo.Arguments = "tcc" + " -o " + Path.GetFileNameWithoutExtension(ActiveDocument.FilePath) + " " + ActiveDocument.FilePath;
            cmd.StartInfo.UseShellExecute = false;
            cmd.StartInfo.FileName = "cmd";
            cmd.StartInfo.RedirectStandardOutput = true;
            cmd.StartInfo.CreateNoWindow = true;

            cmd.Start();

            StreamReader sr = cmd.StandardOutput;
            while (!sr.EndOfStream) {
                String s = sr.ReadLine();
                if (s != "") {
                    statusTextBox.Text += DateTime.Now.ToString() + ": " + s + Environment.NewLine;
                }
                statusTextBox.Refresh();
            }
        }

        private void helpToolStripMenuItem_Click(object sender, EventArgs e) {
            using (Help help = new Help()) {
                help.ShowDialog();
            }
        }

        private void snippetsToolStripMenuItem_Click(object sender, EventArgs e) {
            ActiveDocument.Scintilla.Snippets.ShowSnippetList();
        }
    }
}