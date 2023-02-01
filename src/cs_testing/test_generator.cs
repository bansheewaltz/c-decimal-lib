using System;
using System.IO;

namespace decimal_testing
{
    public static class Program
    {
        const int tests_n = 1000;

        public static int NextInt32(this Random rng)
        {
            int firstBits = rng.Next(0, 1 << 4) << 28;
            int lastBits = rng.Next(0, 1 << 28);
            return firstBits | lastBits;
        }

        public static decimal NextDecimal(this Random rng)
        {
            byte scale = (byte)rng.Next(29);
            bool sign = rng.Next(2) == 1;
            return new decimal(rng.NextInt32(), rng.NextInt32(), rng.NextInt32(), sign, scale);
        }

        public static void check_bins()
        {
            string tests_bin = @"./test_cases/binary_ops.bin";
            string tests_dec = @"./test_cases/binary_ops_decoded.txt";

            var bin_reader = new BinaryReader(File.Open(tests_bin, FileMode.Open));
            var dec_writer = new StreamWriter(File.Open(tests_dec, FileMode.OpenOrCreate));
            for (int i = 0; i < tests_n; ++i)
            {
                dec_writer.WriteLine("{0} {1}", bin_reader.ReadDecimal(), bin_reader.ReadDecimal());
            }
            bin_reader.Close();
            dec_writer.Close();
        }

        public static void mul_result()
        {
            string tests_bin = @"./test_cases/binary_ops.bin";
            string res_mul_bin = @"./test_cases/res_mul.txt";
            string res_mul_dec = @"./test_cases/res_mul.bin";

            var bin_reader = new BinaryReader(File.Open(tests_bin, FileMode.Open));
            var bin_writer = new BinaryWriter(File.Open(res_mul_bin, FileMode.OpenOrCreate));
            var dec_writer = new StreamWriter(File.Open(res_mul_dec, FileMode.OpenOrCreate));

            decimal value;
            for (int i = 0; i < tests_n; ++i)
            {
                value = decimal.Multiply(bin_reader.ReadDecimal(), bin_reader.ReadDecimal());
                bin_writer.Write(value);
                dec_writer.WriteLine(value);
            }
            bin_reader.Close();
            bin_writer.Close();
            dec_writer.Close();
        }

        static void Main(string[] args)
        {
            string tests_bin = @"./test_cases/binary_ops.bin";
            string tests_dec = @"./test_cases/binary_ops.txt";
            var binwriter = new BinaryWriter(File.Open(tests_bin, FileMode.OpenOrCreate));
            var decwriter = new StreamWriter(File.Open(tests_dec, FileMode.OpenOrCreate));

            Random rnd = new Random();
            decimal a;
            decimal b;
            for (int i = 0; i < tests_n; ++i)
            {
                a = rnd.NextDecimal();
                b = rnd.NextDecimal();
                decwriter.Write(a + " ");
                decwriter.WriteLine(b);
                binwriter.Write(a);
                binwriter.Write(b);
            }
            binwriter.Close();
            decwriter.Close();

            check_bins();
            mul_result();
        }
    }
}
