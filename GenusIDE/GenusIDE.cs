#region Using Directives

using System;
using System.Globalization;
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

        #endregion Fields

        #region Methods

        public GenusIDE() {
            InitializeComponent();
        }

        private DocumentForm NewDocument() {
            DocumentForm doc = new DocumentForm();
            //SetScintillaToCurrentOptions(doc);
            doc.Text = String.Format(CultureInfo.CurrentCulture, "{0}{1}", NEW_DOCUMENT_TEXT, ++_newDocumentCount);
            doc.Show(dockPanel);
            // TODO Implement 
            //toolIncremental.Searcher.Scintilla = doc.Scintilla;
            return doc;
        }

        //private void SetScintillaToCurrentOptions(DocumentForm doc) {
        //    // Turn on line numbers?
        //    if (lineNumbersToolStripMenuItem.Checked)
        //        doc.Scintilla.Margins.Margin0.Width = LINE_NUMBERS_MARGIN_WIDTH;
        //    else
        //        doc.Scintilla.Margins.Margin0.Width = 0;

        //    // Turn on white space?
        //    if (whitespaceToolStripMenuItem.Checked)
        //        doc.Scintilla.Whitespace.Mode = WhitespaceMode.VisibleAlways;
        //    else
        //        doc.Scintilla.Whitespace.Mode = WhitespaceMode.Invisible;

        //    // Turn on word wrap?
        //    if (wordWrapToolStripMenuItem.Checked)
        //        doc.Scintilla.LineWrapping.Mode = LineWrappingMode.Word;
        //    else
        //        doc.Scintilla.LineWrapping.Mode = LineWrappingMode.None;

        //    // Show EOL?
        //    doc.Scintilla.EndOfLine.IsVisible = endOfLineToolStripMenuItem.Checked;

        //    // Set the zoom
        //    doc.Scintilla.Zoom = _zoomLevel;
        //}

        private void CToolStripMenuItemClick(object sender, EventArgs e) { }

        private void OpenToolStripMenuItemClick(object sender, EventArgs e) {
            FileDialog dialog = new OpenFileDialog();
            dialog.Title = "Open file...";
            dialog.Filter = Resources.File_Types;
            dialog.ShowDialog();

            {
                var temp = dialog.FileName.Split('\\');

                //var newtabPage = new TabPage(temp.Last());
                //tabControl1.TabPages.Add(newtabPage);

                saveToolStripMenuItem.Text = "S&ave " + temp.Last();
                closetoolStripMenuItem3.Text = "&Close" + temp.Last();

                toolStripStatusLabel1.Text = temp.Last() + " opened.";
            }

            Text = "GenusIDE - " + dialog.FileName;
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
        }

    }
}