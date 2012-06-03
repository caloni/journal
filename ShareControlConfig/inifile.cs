using System;
using System.Runtime.InteropServices;



namespace ShareControlConfig
{
   public class IniFile
   {
      //
      // API functions
      //
      [DllImport("kernel32")]
      private static extern int GetPrivateProfileStringA(
         string lpApplicationName, 
         string lpKeyName, 
         string lpDefault, 
         System.Text.StringBuilder lpReturnedString, 
         int nSize, 
         string lpFileName);

      [DllImport("kernel32")]
      private static extern int WritePrivateProfileStringA(
         string lpApplicationName, 
         string lpKeyName, 
         string lpString, 
         string lpFileName);

      [DllImport("kernel32")]
      private static extern int FlushPrivateProfileStringA(
         int lpApplicationName, 
         int lpKeyName, 
         int lpString, 
         string lpFileName);

      string strFilename;

      // Constructor, accepting a filename
      public IniFile(string Filename)
      {
         strFilename = Filename;
      }

      // Read-only filename property
      string FileName
      {
         get
         {
            return strFilename;
         }
      }

      // Returns a string from your INI file
      public string GetString(string Section, string Key, string Default)
      {
         int intCharCount;
         System.Text.StringBuilder objResult = new System.Text.StringBuilder(260);
   
         intCharCount = GetPrivateProfileStringA(Section, 
            Key, 
            Default, 
            objResult, 
            objResult.Capacity, 
            strFilename);

         return objResult.ToString();
      }

      // Writes a string to your INI file
      public void WriteString(string Section, string Key, string Value)
      {
         WritePrivateProfileStringA(Section, Key, Value, strFilename);
      }
   }
}
