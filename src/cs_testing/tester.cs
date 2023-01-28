using System;

namespace decimal_testing
{
    class Program
    {
        static void Main(string[] args)
        {
            decimal a = Convert.ToDecimal(Console.ReadLine());
            string? operation = Console.ReadLine();
            decimal b = Convert.ToDecimal(Console.ReadLine());
            decimal expected_result = Convert.ToDecimal(Console.ReadLine());

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
                Console.WriteLine("result: {0} {1} {2} = {3}", a, operation, b, result);
                Console.WriteLine("passed: {0} {1} {2} = {3}", a, operation, b, expected_result);
            }
            else
            {
                Console.WriteLine("ok");
            }
        }
    }
}
