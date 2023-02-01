using System;
using System.IO;
using System.Globalization;
using System.Linq;

class Program {
  static decimal[] list = {
    0m,
    -0.1m,
    0.1m,
    -1m,
    1m,
    -1.0m,
    1.0m,
    -1m / 3m,
    1m / 3m,
    -1m / 3m * 3m,
    1m / 3m * 3m,
    -5m / 3m,
    5m / 3m,
    -1.0000000000000000000000000000m,
    1.0000000000000000000000000000m,
    -0.0000000000000000000000000001M,
    0.0000000000000000000000000001M,
    -7.9228162514264337593543950335M,
    +7.9228162514264337593543950335M,
    decimal.MinValue,
    decimal.MaxValue,
  };
  static int list_len = list.Length;
  static void write_binary_op_header(StreamWriter dw) {
    dw.WriteLine($"{"a",16}{"|",17}{"b", 17}{"|",17}{"result", 19}{"|",15}{"return code", 12}");
    var f1 = String.Concat(Enumerable.Repeat("-", 113));
    dw.WriteLine("{0}", f1);
  }
  static void mul_generator() {
    var bw = new BinaryWriter(File.Open(@"./mul.bin", FileMode.Create));
    var dw = new StreamWriter(File.Open(@"./mul.txt", FileMode.Create));
    write_binary_op_header(dw);
    decimal a, b, res = decimal.Zero;
    string sres;
    Int32 code;

    for (int i = 0; i < list_len; ++i) {
      for (int j = i; j < list_len; ++j) {
        a = list[i];
        b = list[j];

        try {
          res = a * b;
          code = 0;
          sres = res.ToString();
        } catch {
          if ((a > 0 && b > 0) || (a < 0 && b < 0)) {
            code = 1;
            sres = "+inf";
          } else {
            code = 2;
            sres = "-inf";
          }
        }
        bw.Write(a);
        bw.Write(b);
        bw.Write(code);
        bw.Write(res);
        dw.WriteLine($"{a,31} * {b,31} = {sres,31} | {code}");
      }
    }
    bw.Close();
    dw.Close();
  }
  static void Main() {
    ConversionSample.print();
    mul_generator();
  }
}

public static class ConversionSample {
  public static void print() {
    string path = @"./conversion_sample.txt";
    var sw = new StreamWriter(File.Open(path, FileMode.Create));

    Decimal[] values = { Decimal.Zero,
                         1M,
                         -1M,
                         0.1M,
                         0.0000000000000000000000000001M,
                         0.0000000000000000000000000000M,
                         -0.1M,
                         0.5M,
                         -0.5M,
                         2M,
                         3M,
                         1M,
                         1.0M,
                         -1.0M,
                         1.00M,
                         1.0000M,
                         1.0000000000000000000000000000M,
                         100000000000000M,
                         10000000000000000000000000000M,
                         100000000000000.00000000000000M,
                         1M / 3M,
                         2M / 3M,
                         1M / 3M * 3M,
                         123456789M,
                         0.123456789M,
                         0.000000000123456789M,
                         0.000000000000000000123456789M,
                         4294967295M,
                         18446744073709551615M,
                         Decimal.MaxValue,
                         Decimal.MinValue,
                         Decimal.MinValue + 1,
                         -7.9228162514264337593543950335M,
                         +7.9228162514264337593543950335M };

    var f1 = String.Concat(Enumerable.Repeat("-", 20));
    var f2 = String.Concat(Enumerable.Repeat("-", 8));
    var f3 = String.Concat(Enumerable.Repeat("-", 32));
    sw.WriteLine(
        "{0,31}  {1,10:X8}{2,10:X8}{3,10:X8}{4,10:X8}    {5,-34}{6,-34}{7,-34}{8,-34}  {9,-32}",
        "Decimal type number", "Bits[3]", "Bits[2]", "Bits[1]", "Bits[0]", "Bits[3]", "Bits[2]",
        "Bits[1]", "Bits[0]", "Decimal type number");
    sw.WriteLine(
        "{0,31}  {1,10:X8}{2,10:X8}{3,10:X8}{4,10:X8}    {5,-34}{6,-34}{7,-34}{8,-34}  {9,-34}", f1,
        f2, f2, f2, f2, f3, f3, f3, f3, f1);

    foreach (var value in values) {
      int[] bits = decimal.GetBits(value);
      sw.WriteLine(
          "{0,31}  {1,10:X8}{2,10:X8}{3,10:X8}{4,10:X8}    {5,-34}{6,-34}{7,-34}{8,-34}  {9,-31}",
          value, bits[3], bits[2], bits[1], bits[0], Convert.ToString(bits[3], 2).PadLeft(32, '0'),
          Convert.ToString(bits[2], 2).PadLeft(32, '0'),
          Convert.ToString(bits[1], 2).PadLeft(32, '0'),
          Convert.ToString(bits[0], 2).PadLeft(32, '0'), value);
    }
    var s1 = String.Concat(Enumerable.Repeat(" ", 77));
    var s2 = String.Concat(Enumerable.Repeat(" ", 73));
    var s3 = String.Concat(Enumerable.Repeat(" ", 78));
    sw.WriteLine("{0}{1}", s1, "|      |<------><-------------->");
    sw.WriteLine("{0}{1}", s1, "|      [exponent]  [not used]");
    sw.WriteLine("{0}{1}", s2, "[sign bit]");
    sw.WriteLine("{0}{1}", s3, "|<---->|");
    sw.WriteLine("{0}{1}", s1, "[not used]");

    sw.Close();
  }
}
