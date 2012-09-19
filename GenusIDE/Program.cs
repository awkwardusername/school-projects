using System;
using System.IO;
using System.Reflection;
using System.Windows.Forms;

namespace GenusIDE {
    internal static class Program {

        private static GenusIDE _genusIDE = null;
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        private static void Main(string[] args) {
            
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            About splash = new About();
            splash.Show();
            Application.Run(new GenusIDE(splash));
        }

        public static string Title {
            get {
                object[] attributes = Assembly.GetExecutingAssembly().GetCustomAttributes(typeof(AssemblyTitleAttribute), false);
                if (attributes.Length > 0) {
                    AssemblyTitleAttribute titleAttribute = (AssemblyTitleAttribute)attributes[0];
                    if (!String.IsNullOrEmpty(titleAttribute.Title))
                        return titleAttribute.Title;
                }

                // If there was no Title attribute, or if the Title attribute was the empty string, return the .exe name
                return Path.GetFileNameWithoutExtension(Assembly.GetExecutingAssembly().CodeBase);
            }
        }
    }
}