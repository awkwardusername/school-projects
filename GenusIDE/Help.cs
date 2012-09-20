using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace GenusIDE {
    public partial class Help : Form {
        public Help() {
            InitializeComponent();
        }

        private void Help_MouseClick(object sender, MouseEventArgs e) {
            Dispose();
        }

        private void Help_KeyPress(object sender, KeyPressEventArgs e) {
            Dispose();
        }
    }
}
