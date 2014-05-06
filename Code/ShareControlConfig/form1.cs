using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace ShareControlConfig
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
      private System.Windows.Forms.ColumnHeader colAccPath;
      private System.Windows.Forms.ColumnHeader colAccRights;
      private System.Windows.Forms.ColumnHeader colAccPriority;
      private System.Windows.Forms.ListView lstAcc;
      private System.Windows.Forms.Label lblPath;
      private System.Windows.Forms.TextBox txtPath;
      private System.Windows.Forms.TrackBar trkPriority;
      private System.Windows.Forms.GroupBox grpRights;
      private System.Windows.Forms.CheckBox chkOpen;
      private System.Windows.Forms.CheckBox chkRead;
      private System.Windows.Forms.CheckBox chkWrite;
      private System.Windows.Forms.CheckBox chkRename;
      private System.Windows.Forms.CheckBox chkDelete;
      private System.Windows.Forms.CheckBox chkRun;
      private System.Windows.Forms.CheckBox chkSubdirectories;
      private System.Windows.Forms.Label lblPriority;
      private System.Windows.Forms.MainMenu mainMenu1;
      private System.Windows.Forms.MenuItem mnuFile;
      private System.Windows.Forms.MenuItem mnuExit;
      private System.Windows.Forms.MenuItem mnuOpen;
      private System.Windows.Forms.MenuItem mnuSave;
      private System.Windows.Forms.MenuItem mnuFileSpc1;
      private System.Windows.Forms.Button btnAdd;
      private System.Windows.Forms.Button btnDelete;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Form1()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

         m_bAccItemSelected = false;
		}
		
		private bool m_bAccItemSelected;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
         System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(Form1));
         this.lstAcc = new System.Windows.Forms.ListView();
         this.colAccPath = new System.Windows.Forms.ColumnHeader();
         this.colAccRights = new System.Windows.Forms.ColumnHeader();
         this.colAccPriority = new System.Windows.Forms.ColumnHeader();
         this.lblPath = new System.Windows.Forms.Label();
         this.txtPath = new System.Windows.Forms.TextBox();
         this.trkPriority = new System.Windows.Forms.TrackBar();
         this.grpRights = new System.Windows.Forms.GroupBox();
         this.chkSubdirectories = new System.Windows.Forms.CheckBox();
         this.chkRename = new System.Windows.Forms.CheckBox();
         this.chkDelete = new System.Windows.Forms.CheckBox();
         this.chkRun = new System.Windows.Forms.CheckBox();
         this.chkWrite = new System.Windows.Forms.CheckBox();
         this.chkRead = new System.Windows.Forms.CheckBox();
         this.chkOpen = new System.Windows.Forms.CheckBox();
         this.lblPriority = new System.Windows.Forms.Label();
         this.mainMenu1 = new System.Windows.Forms.MainMenu();
         this.mnuFile = new System.Windows.Forms.MenuItem();
         this.mnuOpen = new System.Windows.Forms.MenuItem();
         this.mnuSave = new System.Windows.Forms.MenuItem();
         this.mnuFileSpc1 = new System.Windows.Forms.MenuItem();
         this.mnuExit = new System.Windows.Forms.MenuItem();
         this.btnAdd = new System.Windows.Forms.Button();
         this.btnDelete = new System.Windows.Forms.Button();
         ((System.ComponentModel.ISupportInitialize)(this.trkPriority)).BeginInit();
         this.grpRights.SuspendLayout();
         this.SuspendLayout();
         // 
         // lstAcc
         // 
         this.lstAcc.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
         this.lstAcc.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
                                                                                 this.colAccPath,
                                                                                 this.colAccRights,
                                                                                 this.colAccPriority});
         this.lstAcc.FullRowSelect = true;
         this.lstAcc.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
         this.lstAcc.HideSelection = false;
         this.lstAcc.Location = new System.Drawing.Point(8, 216);
         this.lstAcc.Name = "lstAcc";
         this.lstAcc.Size = new System.Drawing.Size(488, 208);
         this.lstAcc.TabIndex = 0;
         this.lstAcc.View = System.Windows.Forms.View.Details;
         this.lstAcc.SelectedIndexChanged += new System.EventHandler(this.lstAcc_SelectedIndexChanged);
         // 
         // colAccPath
         // 
         this.colAccPath.Text = "Path";
         this.colAccPath.Width = 300;
         // 
         // colAccRights
         // 
         this.colAccRights.Text = "Rights";
         this.colAccRights.Width = 120;
         // 
         // colAccPriority
         // 
         this.colAccPriority.Text = "Priority";
         // 
         // lblPath
         // 
         this.lblPath.Enabled = false;
         this.lblPath.Location = new System.Drawing.Point(8, 16);
         this.lblPath.Name = "lblPath";
         this.lblPath.Size = new System.Drawing.Size(32, 23);
         this.lblPath.TabIndex = 1;
         this.lblPath.Text = "&Path";
         this.lblPath.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
         // 
         // txtPath
         // 
         this.txtPath.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
         this.txtPath.Enabled = false;
         this.txtPath.Location = new System.Drawing.Point(40, 16);
         this.txtPath.Name = "txtPath";
         this.txtPath.Size = new System.Drawing.Size(456, 20);
         this.txtPath.TabIndex = 2;
         this.txtPath.Text = "";
         this.txtPath.TextChanged += new System.EventHandler(this.txtPath_TextChanged);
         // 
         // trkPriority
         // 
         this.trkPriority.Enabled = false;
         this.trkPriority.Location = new System.Drawing.Point(424, 72);
         this.trkPriority.Maximum = 100;
         this.trkPriority.Name = "trkPriority";
         this.trkPriority.Orientation = System.Windows.Forms.Orientation.Vertical;
         this.trkPriority.RightToLeft = System.Windows.Forms.RightToLeft.No;
         this.trkPriority.Size = new System.Drawing.Size(45, 88);
         this.trkPriority.TabIndex = 4;
         this.trkPriority.TickFrequency = 10;
         this.trkPriority.TickStyle = System.Windows.Forms.TickStyle.Both;
         this.trkPriority.Value = 50;
         this.trkPriority.ValueChanged += new System.EventHandler(this.trkPriority_ValueChanged);
         // 
         // grpRights
         // 
         this.grpRights.Controls.Add(this.chkSubdirectories);
         this.grpRights.Controls.Add(this.chkRename);
         this.grpRights.Controls.Add(this.chkDelete);
         this.grpRights.Controls.Add(this.chkRun);
         this.grpRights.Controls.Add(this.chkWrite);
         this.grpRights.Controls.Add(this.chkRead);
         this.grpRights.Controls.Add(this.chkOpen);
         this.grpRights.Enabled = false;
         this.grpRights.Location = new System.Drawing.Point(8, 56);
         this.grpRights.Name = "grpRights";
         this.grpRights.Size = new System.Drawing.Size(408, 104);
         this.grpRights.TabIndex = 5;
         this.grpRights.TabStop = false;
         this.grpRights.Text = "R&ights";
         // 
         // chkSubdirectories
         // 
         this.chkSubdirectories.Location = new System.Drawing.Point(288, 24);
         this.chkSubdirectories.Name = "chkSubdirectories";
         this.chkSubdirectories.TabIndex = 6;
         this.chkSubdirectories.Text = "&Subdirectories";
         this.chkSubdirectories.CheckedChanged += new System.EventHandler(this.chkAcc_CheckedChanged);
         // 
         // chkRename
         // 
         this.chkRename.Location = new System.Drawing.Point(152, 72);
         this.chkRename.Name = "chkRename";
         this.chkRename.TabIndex = 5;
         this.chkRename.Text = "Re&name";
         this.chkRename.CheckedChanged += new System.EventHandler(this.chkAcc_CheckedChanged);
         // 
         // chkDelete
         // 
         this.chkDelete.Location = new System.Drawing.Point(152, 48);
         this.chkDelete.Name = "chkDelete";
         this.chkDelete.TabIndex = 4;
         this.chkDelete.Text = "&Delete";
         this.chkDelete.CheckedChanged += new System.EventHandler(this.chkAcc_CheckedChanged);
         // 
         // chkRun
         // 
         this.chkRun.Location = new System.Drawing.Point(152, 24);
         this.chkRun.Name = "chkRun";
         this.chkRun.TabIndex = 3;
         this.chkRun.Text = "E&xecute";
         this.chkRun.CheckedChanged += new System.EventHandler(this.chkAcc_CheckedChanged);
         // 
         // chkWrite
         // 
         this.chkWrite.Location = new System.Drawing.Point(16, 72);
         this.chkWrite.Name = "chkWrite";
         this.chkWrite.TabIndex = 2;
         this.chkWrite.Text = "&Write";
         this.chkWrite.CheckedChanged += new System.EventHandler(this.chkAcc_CheckedChanged);
         // 
         // chkRead
         // 
         this.chkRead.Location = new System.Drawing.Point(16, 48);
         this.chkRead.Name = "chkRead";
         this.chkRead.TabIndex = 1;
         this.chkRead.Text = "&Read";
         this.chkRead.CheckedChanged += new System.EventHandler(this.chkAcc_CheckedChanged);
         // 
         // chkOpen
         // 
         this.chkOpen.Location = new System.Drawing.Point(16, 24);
         this.chkOpen.Name = "chkOpen";
         this.chkOpen.TabIndex = 0;
         this.chkOpen.Text = "&Open";
         this.chkOpen.CheckedChanged += new System.EventHandler(this.chkAcc_CheckedChanged);
         // 
         // lblPriority
         // 
         this.lblPriority.Enabled = false;
         this.lblPriority.Location = new System.Drawing.Point(424, 56);
         this.lblPriority.Name = "lblPriority";
         this.lblPriority.Size = new System.Drawing.Size(45, 23);
         this.lblPriority.TabIndex = 6;
         this.lblPriority.Text = "50";
         this.lblPriority.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
         // 
         // mainMenu1
         // 
         this.mainMenu1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
                                                                                  this.mnuFile});
         // 
         // mnuFile
         // 
         this.mnuFile.Index = 0;
         this.mnuFile.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
                                                                                this.mnuOpen,
                                                                                this.mnuSave,
                                                                                this.mnuFileSpc1,
                                                                                this.mnuExit});
         this.mnuFile.Text = "&File";
         // 
         // mnuOpen
         // 
         this.mnuOpen.Index = 0;
         this.mnuOpen.Text = "&Open...";
         this.mnuOpen.Click += new System.EventHandler(this.mnuOpen_Click);
         // 
         // mnuSave
         // 
         this.mnuSave.Index = 1;
         this.mnuSave.Text = "&Save...";
         this.mnuSave.Click += new System.EventHandler(this.mnuSave_Click);
         // 
         // mnuFileSpc1
         // 
         this.mnuFileSpc1.Index = 2;
         this.mnuFileSpc1.Text = "-";
         // 
         // mnuExit
         // 
         this.mnuExit.Index = 3;
         this.mnuExit.Text = "E&xit";
         this.mnuExit.Click += new System.EventHandler(this.mnuExit_Click);
         // 
         // btnAdd
         // 
         this.btnAdd.Location = new System.Drawing.Point(8, 176);
         this.btnAdd.Name = "btnAdd";
         this.btnAdd.TabIndex = 7;
         this.btnAdd.Text = "Add";
         this.btnAdd.Click += new System.EventHandler(this.btnAdd_Click);
         // 
         // btnDelete
         // 
         this.btnDelete.Enabled = false;
         this.btnDelete.Location = new System.Drawing.Point(96, 176);
         this.btnDelete.Name = "btnDelete";
         this.btnDelete.TabIndex = 8;
         this.btnDelete.Text = "Delete";
         this.btnDelete.Click += new System.EventHandler(this.btnDelete_Click);
         // 
         // Form1
         // 
         this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
         this.ClientSize = new System.Drawing.Size(504, 430);
         this.Controls.Add(this.btnDelete);
         this.Controls.Add(this.btnAdd);
         this.Controls.Add(this.lblPriority);
         this.Controls.Add(this.grpRights);
         this.Controls.Add(this.trkPriority);
         this.Controls.Add(this.txtPath);
         this.Controls.Add(this.lblPath);
         this.Controls.Add(this.lstAcc);
         this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
         this.Menu = this.mainMenu1;
         this.Name = "Form1";
         this.Text = "ShareControl Configurator Tabajara";
         ((System.ComponentModel.ISupportInitialize)(this.trkPriority)).EndInit();
         this.grpRights.ResumeLayout(false);
         this.ResumeLayout(false);

      }
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new Form1());
		}

      private void lstAcc_SelectedIndexChanged(object sender, System.EventArgs e)
      {
         bool b = lstAcc.SelectedItems.Count != 0;

         EnableAccCfgControls(b);
         
         m_bAccItemSelected = false;
         FillAccCfgControls();
         m_bAccItemSelected = b;
      }

      private void EnableAccCfgControls(bool bEnable)
      {
         lblPath.Enabled = bEnable;
         txtPath.Enabled = bEnable;
         lblPriority.Enabled = bEnable;
         lblPriority.Enabled = bEnable;
         trkPriority.Enabled = bEnable;
         grpRights.Enabled = bEnable;
         btnDelete.Enabled = bEnable;
      }

      private void FillAccCfgControls()
      {
         if( lstAcc.SelectedItems.Count == 0 )
         {
            txtPath.Text = "";
            trkPriority.Value = 50;
            foreach( CheckBox i in grpRights.Controls ) i.Checked = false;
         }
         else
         {
            ListViewItem i = lstAcc.SelectedItems[0];
            txtPath.Text = i.Text;
            trkPriority.Value = int.Parse(i.SubItems[2].Text);
            
            ListViewItem.ListViewSubItem si = i.SubItems[1];
            foreach( CheckBox j in grpRights.Controls )
            {
               int ind = j.Text.IndexOf('&') + 1;
               if( ind != 0 )
               {
                  char c = Char.ToUpper(j.Text[ind]);
                  j.Checked = si.Text.IndexOf(c) != -1;
               }
               else j.Checked = false;
            }
         }
      }

      private void trkPriority_ValueChanged(object sender, System.EventArgs e)
      {
         lblPriority.Text = trkPriority.Value.ToString();

         if( m_bAccItemSelected )
            lstAcc.SelectedItems[0].SubItems[2].Text = lblPriority.Text;
      }

      private void chkAcc_CheckedChanged(object sender, System.EventArgs e)
      {
         if( m_bAccItemSelected )
         {
            string rights = "";
            foreach( CheckBox i in grpRights.Controls )
            {
               if( i.Checked )
               {
                  int ind = i.Text.IndexOf('&') + 1;
                  if( ind != 0 )
                     rights += Char.ToUpper(i.Text[ind]);
               }
            }
            lstAcc.SelectedItems[0].SubItems[1].Text = rights;
         }
      }

      private void mnuExit_Click(object sender, System.EventArgs e)
      {
         Close();
      }

      private void mnuOpen_Click(object sender, System.EventArgs e)
      {
         OpenFileDialog ofd = new OpenFileDialog();
         ofd.Filter = "Share Control Config Files (*.ini*)|*.ini";
         if( ofd.ShowDialog() == DialogResult.OK )
            LoadCfgFile(ofd.FileName);
      }

      private void mnuSave_Click(object sender, System.EventArgs e)
      {
         SaveFileDialog sfd = new SaveFileDialog();
         sfd.Filter = "Share Control Config Files (*.ini*)|*.ini";
         if( sfd.ShowDialog() == DialogResult.OK )
            SaveCfgFile(sfd.FileName);
      }

      private void LoadCfgFile(string FileName)
      {
         IniFile ini = new IniFile(FileName);
         string path, rights, priority;

         for( int i = 1; i < 1000; ++i )
         {
            path = ini.GetString("ACC" + i, "Path", "");
            rights = ini.GetString("ACC" + i, "Controle", "");
            priority = ini.GetString("ACC" + i, "Prioridade", "");
            
            if( path == "" ) break;
            
            ListViewItem it = new ListViewItem(path);
            it.SubItems.Add(rights);
            it.SubItems.Add(priority);
            
            lstAcc.Items.Add(it);
         }
      }

      private void SaveCfgFile(string FileName)
      {
         IniFile ini = new IniFile(FileName);
         string path, rights, priority;

         foreach( ListViewItem it in lstAcc.Items )
         {
            path = it.Text;
            rights = it.SubItems[1].Text;
            priority = it.SubItems[2].Text;

            ini.WriteString("ACC" + (it.Index + 1), "Path", path);
            ini.WriteString("ACC" + (it.Index + 1), "Acesso", rights);
            ini.WriteString("ACC" + (it.Index + 1), "Prioridade", priority);
         }
      }

      private void txtPath_TextChanged(object sender, System.EventArgs e)
      {
         if( m_bAccItemSelected )
            lstAcc.SelectedItems[0].Text = txtPath.Text;
      }

      private void btnAdd_Click(object sender, System.EventArgs e)
      {
         ListViewItem it = new ListViewItem("?\\*.*");
         it.SubItems.Add("");
         it.SubItems.Add("50");

         lstAcc.Items.Add(it);
         lstAcc.SelectedItems.Clear();
         it.Selected = true;
      }

      private void btnDelete_Click(object sender, System.EventArgs e)
      {
         if( m_bAccItemSelected )
         {
            int i = lstAcc.SelectedItems[0].Index;
            lstAcc.SelectedItems[0].Remove();
            if( i < lstAcc.Items.Count )
               lstAcc.Items[i].Selected = true;
         }
      }
	}
}
