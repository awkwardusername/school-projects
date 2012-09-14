using System.Windows.Forms;

namespace GenusIDE {
    public partial class About : Form {
        public About() {
            InitializeComponent();
        }

        private void About_MouseClick(object sender, MouseEventArgs e) {
            Dispose();
        }

        private void About_KeyPress(object sender, KeyPressEventArgs e) {
            Dispose();
        }
    }
}