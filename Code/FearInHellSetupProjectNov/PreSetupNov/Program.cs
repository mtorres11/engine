using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics;   

namespace PreSetupNov
{
    class Program
    {
        /// <summary>
        /// Local file path to 7sip application.
        /// </summary>
        static string sevenzip = @"..\..\..\..\Assets\Utils\7-Zip\7z.exe";
           
        /// <summary>
        /// Local path where the .zip retrieved from the buildserver containing the application will be decompressed.
        /// </summary>
        static string ToolsLocalPath = @"QuickCaptureTemplate\";
         
        /// <summary>
        /// Local path where the files needed for the creation of the setup file will be copied to.
        /// </summary>
        static string SetupFiles = @"SetupFiles\";
         
        static void Main(string[] args)
        {
            try
            {
                // Delete dummy target
                if (File.Exists(@"..\..\..\..\Bin\Test\FearInHell.exe"))
                { 
                    File.Delete(@"..\..\..\..\Bin\Test\FearInHell.exe");
                }

                

                // Make a copy of source file. Change target filename
                File.Copy(@"..\..\..\..\Bin\Test\Test.exe", @"..\..\..\..\Bin\Test\FearInHell.exe");

                // Delete source file
                //File.Delete(@"..\..\..\..\Bin\Test\Test.exe");



            }
            catch (Exception ex)
            {
                TextWriter tw = new StreamWriter("log.txt");
                tw.WriteLine(ex.Message);
                tw.Close();
            }
        }

        public static void CopyDirectoryRecursive(string source, string target)
        {
            CopyDirectoryRecursive(new DirectoryInfo(source), new DirectoryInfo(target));
        }

        public static void CopyDirectoryRecursive(DirectoryInfo source, DirectoryInfo target)
        {
            // Check if the target directory exists, if not, create it.
            if (Directory.Exists(target.FullName) == false)
                Directory.CreateDirectory(target.FullName);

            // Copy each file into it’s new directory.
            foreach (FileInfo fi in source.GetFiles())
                fi.CopyTo(Path.Combine(target.ToString(), fi.Name), true);

            // Copy each subdirectory using recursion.
            foreach (DirectoryInfo diSourceSubDir in source.GetDirectories())
            {
                DirectoryInfo nextTargetSubDir = target.CreateSubdirectory(diSourceSubDir.Name);
                CopyDirectoryRecursive(diSourceSubDir, nextTargetSubDir);
            }
        }
    }
}
