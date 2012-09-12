using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GenusIDE {
    public partial class GenusIDE : Form {
        public GenusIDE() {
            InitializeComponent();
        }

        private void CToolStripMenuItemClick(object sender, EventArgs e) {

        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e) {
            FileDialog dialog = new OpenFileDialog();
            dialog.Title = "Open file...";

            dialog.ShowDialog();

            {
                String[] temp = dialog.FileName.Split('\\');
                tabPage1.Text = temp.Last();
                saveToolStripMenuItem.Text = "S&ave " + temp.Last();
            }

            this.Text = "GenusIDE - " + dialog.FileName;
            
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e) {
            this.Dispose();
        }
    }
}
