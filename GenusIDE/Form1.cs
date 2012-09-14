using System;
using System.Linq;
using System.Windows.Forms;
using GenusIDE.Properties;

namespace GenusIDE {
    public partial class GenusIDE : Form {
        public GenusIDE() {
            InitializeComponent();
        }

        private void CToolStripMenuItemClick(object sender, EventArgs e) {}

        private void OpenToolStripMenuItemClick(object sender, EventArgs e) {
            FileDialog dialog = new OpenFileDialog();
            dialog.Title = "Open file...";
            dialog.Filter = Resources.File_Types;
            dialog.ShowDialog();

            {
                string[] temp = dialog.FileName.Split('\\');

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
            {
                var about = new About();
                about.Show();
            }
        }
    }
}