using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics;

namespace PostSetupNov
{
    class Program
    {
        /// <summary>
        /// Local file path to 7sip application.
        /// </summary>
        static string sevenZipLocaPath = @"..\..\..\..\Assets\Utils\7-Zip\7z.exe";

        static void Main(string[] args)
        {
            try
            {
                // Compress .msi
                //string arguments = "a -tzip FearInHellSetup.zip SetupFile.msi";
                //Process p = Process.Start(sevenZipLocaPath, arguments);
                //p.WaitForExit(60000);

                //// Delete remaining files
                //File.Delete("setup.exe");
            }
            catch (Exception ex)
            {
                TextWriter tw = new StreamWriter("log.txt");
                tw.WriteLine(ex.Message);
                tw.Close();
            }
        }
    }
}
