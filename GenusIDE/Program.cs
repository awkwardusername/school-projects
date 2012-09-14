using System;
using System.Windows.Forms;

namespace GenusIDE {
    internal static class Program {
        public static string Title = "GenusIDE";

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        private static void Main() {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new GenusIDE());
        }
    }
}