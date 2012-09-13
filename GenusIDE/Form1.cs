using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using GenusIDE.Properties;

namespace GenusIDE {
    public partial class GenusIDE : Form {
        public GenusIDE() {
            InitializeComponent();
        }

        private void CToolStripMenuItemClick(object sender, EventArgs e) {

        }

        private void OpenToolStripMenuItemClick(object sender, EventArgs e) {
            FileDialog dialog = new OpenFileDialog();
            dialog.Title = "Open file...";
            dialog.Filter = Resources.File_Types;
            dialog.ShowDialog();

            {
                var temp = dialog.FileName.Split('\\');

                var newtabPage = new TabPage(temp.Last());
                //tabControl1.TabPages.Add(newtabPage);

                saveToolStripMenuItem.Text = "S&ave " + temp.Last();
                closetoolStripMenuItem3.Text = "&Close" + temp.Last();

                toolStripStatusLabel1.Text = temp.Last() + " opened.";
            }

            this.Text = "GenusIDE - " + dialog.FileName;
            
        }

        private void ExitToolStripMenuItemClick(object sender, EventArgs e) {
            this.Dispose();
        }
    }
}
