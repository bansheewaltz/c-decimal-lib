using System;
using System.IO;

namespace decimal_testing
{
    class Program
    {
        static void Main(string[] args)
        {
            string filename = @"./test_sample.txt";
            StreamReader sr = File.OpenText(filename);

            decimal a = Convert.ToDecimal(sr.ReadLine());
            string operation = sr.ReadLine();
            decimal b = Convert.ToDecimal(sr.ReadLine());
            decimal expected_result = Convert.ToDecimal(sr.ReadLine());

            decimal result = 0;
            switch (operation)
            {
                case "+":
                    result = a + b;
                    break;
                case "-":
                    result = a - b;
                    break;
                case "*":
                    result = a * b;
                    break;
                case "/":
                    result = a / b;
                    break;
                default:
                    Console.WriteLine("Wrong input");
                    break;
            }

            if (result != expected_result)
            {
                Console.WriteLine("fail");
                Console.WriteLine("test: {0} {1} {2}", a, operation, b);
                Console.WriteLine("result: {0}", result);
                Console.WriteLine("passed: {0}", expected_result);
            }
            else
            {
                Console.WriteLine("ok");
            }
        }
    }
}
