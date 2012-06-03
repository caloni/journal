using System;
using System.Collections;

namespace CDMgr
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	class CMain
	{
	   delegate int Command(string[] args);
	
	   static void PrintHelp(string exe)
	   {
	      Console.WriteLine("CDs Manager very beta edition.");
	      Console.WriteLine("How to use: {0} cmd params", exe);
	      Console.WriteLine("Options:");
	      Console.WriteLine("listcds");
	      Console.WriteLine("listfiles \"CD Name\"");
	      Console.WriteLine("findfile \"FileName\"");
	      Console.WriteLine("addcd \"CD Name\" \"CD Root\"");
	      Console.WriteLine("updatecd \"CD Name\" \"CD Root\"");
	      Console.WriteLine("deletecd \"CD Name\"");
	      Console.WriteLine("quit (obs: only in prompt mode)");
	      Console.WriteLine();
	   }

	   static Command GetCommand(string cmd)
	   {
         if( cmd == "listcds" )    return new Command(ListCDs);
         if( cmd == "listfiles" )  return new Command(ListFiles);
         if( cmd == "findfile" )   return new Command(FindFile);
         if( cmd == "addcd" )      return new Command(AddCD);
	      if( cmd == "updatecd" )   return new Command(UpdateCD);
	      if( cmd == "deletecd" )   return new Command(DeleteCD);
	      return new Command(HowToUse);
	   }

      static int ListCDs(string[] args)
      {
         CDMgr cdmgr = new CDMgr();
         if( args.Length >= 1 )
            cdmgr.ReadCDs(args[0]);
         else
            cdmgr.ReadCDs();
         foreach( string s in cdmgr.CDs )
            Console.WriteLine(s);
         return cdmgr.CDs.Count;
      }
      
      static int ListFiles(string[] args)
      {
         if( args.Length >= 1 )
         {
            CDMgr cdmgr = new CDMgr();
            cdmgr.ReadFiles(args[0]);
            foreach( string s in cdmgr.Files )
               Console.WriteLine(s);
            return cdmgr.Files.Count;
         }
         return -1;
      }
      
      static int FindFile(string[] args)
      {
         if( args.Length >= 1 )
         {
            CDMgr cdmgr = new CDMgr();
            cdmgr.FindFiles(args[0]);
            foreach( string s in cdmgr.Files )
               Console.WriteLine(s);
            return cdmgr.Files.Count;
         }
         return -1;
      }

	   static int AddCD(string[] args)
	   {
	      if( args.Length >= 2 )
	      {
	         CDMgr cdmgr = new CDMgr();
	         return cdmgr.Insert(args[0], args[1], true);
	      }
	      return -1;
	   }
	
      static int UpdateCD(string[] args)
      {
         if( args.Length >= 2 )
         {
            CDMgr cdmgr = new CDMgr();
            return cdmgr.Update(args[0], args[1], true);
         }
         return -1;
      }
      
      static int DeleteCD(string[] args)
      {
         if( args.Length >= 1 )
         {
            CDMgr cdmgr = new CDMgr();
            return cdmgr.Delete(args[0]);
         }
         return -1;
      }
      
      static int HowToUse(string[] args)
      {
         PrintHelp("cdmgr");
         return 0;
      }

      static string[] GetCmdArgs(string[] args)
      {
         string[] subargs = new string[args.Length - 1];
         for( int i = 0; i < subargs.Length; ++i )
            subargs[i] = args[i + 1];
         return subargs;
      }

      static string[] CmdToArgs(string cmd)
      {
			string [] ret = new string[1] { "help" };
         ArrayList args = new ArrayList();

         cmd.Trim();
         for( int beg = 0, cur = 0; cur < cmd.Length; cur++ )
         {
            if( cmd[beg] == '\"' && cmd[cur] == '\"' )
            {
               if( ++cur < cmd.Length && cmd[cur] == ' ' )
               {
                  args.Add(cmd.Substring(beg + 1, cur - 2));
                  beg = cur + 1;
               }
            }
            else
            if( cmd[cur] == ' ' )
            {
               args.Add(cmd.Substring(beg, cur - 1));
               beg = cur + 1;
            }
         }
         
         if( args.Count != 0 )
			{
				ret = new string[args.Count];
				args.CopyTo(ret);
			}
         
         return ret;
      }

      static void PromptMode()
      {
         for( ; ; )
         {
            string cmd = Console.ReadLine();

            if( cmd == "quit" )
               break;
   
            ParseArgs(CmdToArgs(cmd));
         }
      }

      static void ParseArgs(string[] args)
      {
         try
         {
            string[] subargs = GetCmdArgs(args);
            Environment.ExitCode = GetCommand(args[0])(subargs);
         }
         catch(System.Data.DataException err)
         {
            Console.WriteLine(err.Message);
            return;
         }
         catch(Exception err)
         {
            Console.WriteLine(err.Message);
            return;
         }
            
         Console.WriteLine(Environment.ExitCode);
      }

      /// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
		   if( args.Length < 1 )
		      PromptMode();
		   else
		      ParseArgs(args);
      }
	}
}
