﻿namespace GenusIDE {
    partial class DocumentForm {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.scintilla = new ScintillaNET.Scintilla();
            ((System.ComponentModel.ISupportInitialize)(this.scintilla)).BeginInit();
            this.SuspendLayout();
            // 
            // scintilla
            // 
            this.scintilla.Dock = System.Windows.Forms.DockStyle.Fill;
            this.scintilla.Font = new System.Drawing.Font("Consolas", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.scintilla.LineWrapping.VisualFlags = ScintillaNET.LineWrappingVisualFlags.End;
            this.scintilla.Location = new System.Drawing.Point(0, 0);
            this.scintilla.Margins.Margin1.AutoToggleMarkerNumber = 0;
            this.scintilla.Margins.Margin1.IsClickable = true;
            this.scintilla.Margins.Margin2.Width = 16;
            this.scintilla.Name = "scintilla";
            this.scintilla.Size = new System.Drawing.Size(292, 272);
            this.scintilla.TabIndex = 0;
            this.scintilla.ModifiedChanged += new System.EventHandler(this.scintilla_ModifiedChanged);
            this.scintilla.StyleNeeded += new System.EventHandler<ScintillaNET.StyleNeededEventArgs>(this.scintilla_StyleNeeded);
            // 
            // DocumentForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 272);
            this.Controls.Add(this.scintilla);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Name = "DocumentForm";
            this.Text = "DocumentForm";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.DocumentForm_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.scintilla)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private ScintillaNET.Scintilla scintilla;
    }
}