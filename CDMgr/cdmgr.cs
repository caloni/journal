using System;
using System.Data;
using System.Data.OleDb;
using System.Collections;
using System.IO;

namespace CDMgr
{
	/// <summary>
	/// Summary description for cdmgr.
	/// </summary>
	public class CDMgr
	{
		public CDMgr()
		{
			//
			// TODO: Add constructor logic here
			//
		}

      public ArrayList Files
      {
         get
         {
            return m_Files;
         }
      }
      
      public ArrayList CDs
      {
         get
         {
            return m_CDs;
         }
      }

      public int FindFiles(string sFile)
      {
         string connStr = ConnectionString;
         OleDbConnection conn = new OleDbConnection(connStr);
         OleDbDataAdapter da = new OleDbDataAdapter();
         DataTable dt = new DataTable("Files");

         conn.Open();

         sFile = sFile.Replace('?', '_');
         sFile = sFile.Replace('*', '%');

         string sel = string.Format("SELECT Name, ID FROM " + 
            "Files WHERE Name LIKE \'{0}\'", sFile);

         da.SelectCommand = new OleDbCommand(sel, conn);
         int cdInfo = da.Fill(dt);

         conn.Close();

         if( cdInfo != 0 )
         {
            string cdName = "Not Found";
            int lastCDID = -1;

            foreach(DataRow dr in dt.Rows)
            {
               int CDID = int.Parse(dr["ID"].ToString());
               if( lastCDID != CDID )
               {
                  lastCDID = CDID;
                  cdName = (string) GetCDNameFromCDID(CDID);
               }

               m_Files.Add(string.Format("\"{0}\" - \"{1}\"",
                  cdName, dr["Name"]));
            }
         }

         return cdInfo;
      }

      public int ReadCDs()
      {  
         return ReadCDs("");
      }

      public int ReadCDs(string cdName)
      {
         string connStr = ConnectionString;
         OleDbConnection conn = new OleDbConnection(connStr);
         OleDbDataAdapter da = new OleDbDataAdapter();
         DataTable dt = new DataTable("CDs");

         conn.Open();

         da.SelectCommand = 
            new OleDbCommand(string.Format(
               "SELECT ID, Name FROM CDs WHERE Name LIKE \'%{0}%\'", 
               cdName),
               conn);
         int cdInfo = da.Fill(dt);

         conn.Close();

         foreach(DataRow dr in dt.Rows)
            m_CDs.Add(string.Format("{0}", dr["Name"]));

         return cdInfo;
      }

      public int ReadFiles(string cdName)
      {
         object cdId = GetCDIDFromCDName(cdName);
         if( cdId != null )
            return ReadFiles( (int) cdId);
         return 0;
      }

      public int ReadFiles(int cdId)
      {
         string connStr = ConnectionString;
         OleDbConnection conn = new OleDbConnection(connStr);
         OleDbDataAdapter da = new OleDbDataAdapter();
         DataTable dt = new DataTable("Files");

         conn.Open();

         string sel = string.Format("SELECT Name FROM " + 
            "Files WHERE Files.ID = {0}", cdId);

         da.SelectCommand = new OleDbCommand(sel, conn);
         int cdInfo = da.Fill(dt);

         conn.Close();

         if( cdInfo != 0 )
         {
            object cdName = GetCDNameFromCDID(cdId);
            if( cdName == null )
               cdName = "EmptyName";

            foreach(DataRow dr in dt.Rows)
            {
               m_Files.Add(string.Format("\"{0}\" - \"{1}\"",
                     cdName, dr["Name"]));
            }
         }

         return cdInfo;
      }

      public int Insert(string cdName, string root, bool recursive)
      {
         ReadFiles(root, recursive);
         return Insert(cdName);
      }

      public int Update(string cdName, string root, bool recursive)
      {
         int ret = Delete(cdName);
         return ret + Insert(cdName, root, recursive);
      }

      public int Insert(string cdName)
      {
         string connStr = ConnectionString;
         OleDbConnection conn = new OleDbConnection(connStr);
         OleDbCommand cmd = new OleDbCommand("", conn);

         conn.Open();
         
         // creates a new cd entry
         cmd.CommandText = 
               string.Format("INSERT INTO CDs (Name) VALUES ('{0}')", 
               cdName);
         int cdInfo = cmd.ExecuteNonQuery();
         
         if( cdInfo != 0 )
         {
            cmd.CommandText = 
                  string.Format("SELECT ID FROM CDs WHERE Name = '{0}'", 
                  cdName);
            cdInfo = (int) cmd.ExecuteScalar();

            if( cdInfo != 0 )
            {
               int entries = 0;

               foreach(string i in Files)
               {
                  string fileName = i.Replace("'", "''");

                  cmd.CommandText = 
                     string.Format(
                     "INSERT INTO Files (ID, Name) " + 
                     "VALUES ({0}, '{1}');",
                     cdInfo, fileName);

                  if( cmd.ExecuteNonQuery() == 0 ) break;
                  ++entries;
               }

               return entries;
            }
         }

         conn.Close();
         return 0;
      }

      public int Delete(string cdName)
      {
         object cdId = GetCDIDFromCDName(cdName);
         if( cdId != null )
            return Delete( (int) cdId);
         return 0;
      }

      public int Delete(int cdId)
      {
         string connStr = ConnectionString;
         OleDbConnection conn = new OleDbConnection(connStr);
         OleDbCommand cmd = new OleDbCommand("", conn);

         conn.Open();

         // delete all file entries from the specified cd
         cmd.CommandText = 
               string.Format("DELETE FROM Files WHERE ID = {0}", cdId);
         int cdInfo = cmd.ExecuteNonQuery();

         // remove the cd entry
         cmd.CommandText = 
               string.Format("DELETE FROM CDs WHERE ID = {0}", cdId);
         cdInfo += (int) cmd.ExecuteNonQuery();

         conn.Close();
         return cdInfo;
      }

		public void ReadFiles(string root, bool recursive)
		{
		   DirectoryInfo dirInfo = new DirectoryInfo(root);
         FileSystemInfo [] files = dirInfo.GetFileSystemInfos();

         foreach(FileSystemInfo file in files)
         {
            if( (file.Attributes & FileAttributes.Directory) != 0 ) continue;

            string fileName = file.FullName;
            int ind = fileName.IndexOf('\\');
            if( ind != -1 )
               fileName = fileName.Substring(ind);
            m_Files.Add(fileName);
         }

         if( recursive )
         {
            DirectoryInfo [] subDirs = dirInfo.GetDirectories();
		   
		      foreach(DirectoryInfo di in subDirs)
		      {
		         ReadFiles(di.FullName, true);
		      }
		   }
		}
		
		protected object GetCDIDFromCDName(string cdName)
		{
         string connStr = ConnectionString;
         OleDbConnection conn = new OleDbConnection(connStr);
         OleDbCommand cmd = new OleDbCommand("", conn);

         conn.Open();

         // get the cd id from its name
         cmd.CommandText = 
            string.Format("SELECT ID FROM CDs WHERE Name = '{0}'", cdName);
         object cdId = cmd.ExecuteScalar();

         conn.Close();
         
         return cdId;
      }

      protected object GetCDNameFromCDID(int cdId)
      {
         string connStr = ConnectionString;
         OleDbConnection conn = new OleDbConnection(connStr);
         OleDbCommand cmd = new OleDbCommand("", conn);

         conn.Open();

         // get the cd id from its name
         cmd.CommandText = 
            string.Format("SELECT Name FROM CDs WHERE ID = {0}", cdId);
         object cdName = cmd.ExecuteScalar();

         conn.Close();
         
         return cdName;
      }

      protected string ConnectionString
      {
         get
         {
            return string.Format(@"Data Source=""{0}"";Provider={1};", 
                  m_DataSource, m_DataProvider);
         }
      }

		private string m_DataSource = @"C:\CDs\CDs.mdb";
		private string m_DataProvider = @"""Microsoft.Jet.OLEDB.4.0""";
		private ArrayList m_Files = new ArrayList();
		private ArrayList m_CDs = new ArrayList();
	}
}
