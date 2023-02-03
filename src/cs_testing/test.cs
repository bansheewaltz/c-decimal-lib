using System;
using System.IO;
using System.Globalization;
using System.Linq;
using System.Collections.Generic;
using static Generator;

static class Tester {
  public static string tests_path = "./test_cases";
  public static int n_unary_op_cases = 50;
  static void Main() {
    // SimpleTest.mod();
    ConversionSample.print();

    // CommutativeBinaryOpTests("mul");
    // CommutativeBinaryOpTests("add");
    // NCommutativeBinaryOpTests("sub");
    // NCommutativeBinaryOpTests("div");
    NCommutativeBinaryOpTests("mod");

    Generator.AddGeneratedUnaryOpCases();
    UnaryOpTests("negate");
  }
  public static List<decimal> cases = new List<decimal>() {
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
  public static int cases_n = cases.Count;

  static void write_binary_op_header(StreamWriter dw) {
    dw.WriteLine($"{"a",16}{"|",17}{"b", 17}{"|",17}{"result", 19}{"|",15}{"return code", 12}");
    var f1 = String.Concat(Enumerable.Repeat("-", 113));
    dw.WriteLine("{0}", f1);
  }

  static bool SignIsEqual(decimal a, decimal b) {
    return Math.Sign(a) == Math.Sign(b);
  }

  static void CommutativeBinaryOpTests(string op) {
    var bw = new BinaryWriter(File.Open($"{tests_path}/{op}.bin", FileMode.Create));
    var dw = new StreamWriter(File.Open($"{tests_path}/{op}.txt", FileMode.Create));
    write_binary_op_header(dw);
    decimal a, b, res = decimal.Zero;
    string sres, symbol = "?";
    Int32 code;

    for (int i = 0; i < cases_n; ++i) {
      for (int j = i; j < cases_n; ++j) {
        a = cases[i];
        b = cases[j];

        try {
          switch (op) {
            case "add":
              symbol = "+";
              res = a + b;
              break;
            case "mul":
              symbol = "*";
              res = a * b;
              break;
            default:
              Console.WriteLine("Incorrect function \"CommutativeBinaryOpTests\" input");
              System.Environment.Exit(1);
              break;
          }
          code = 0;
          sres = res.ToString();
        } catch {
          if ((op == "mul" && SignIsEqual(a, b)) ||
              (op == "add" && SignIsEqual(a, b) && Math.Sign(a) == 1)) {
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
        dw.WriteLine($"{a,31} {symbol} {b,31} = {sres,31} | {code}");
      }
    }
    bw.Close();
    dw.Close();
  }
  static void NCommutativeBinaryOpTests(string op) {
    var bw = new BinaryWriter(File.Open($"{tests_path}/{op}.bin", FileMode.Create));
    var dw = new StreamWriter(File.Open($"{tests_path}/{op}.txt", FileMode.Create));
    write_binary_op_header(dw);
    decimal a, b, res = decimal.Zero;
    string sres, symbol = "?";
    Int32 code;

    for (int i = 0; i < cases_n; ++i) {
      for (int j = 0; j < cases_n; ++j) {
        a = cases[i];
        b = cases[j];

        try {
          switch (op) {
            case "sub":
              symbol = "-";
              res = a - b;
              break;
            case "div":
              symbol = "/";
              res = a / b;
              break;
            case "mod":
              symbol = "%";
              res = a % b;
              break;
            default:
              Console.WriteLine("Incorrect function \"NCommutativeBinaryOpTests\" input");
              System.Environment.Exit(1);
              break;
          }
          code = 0;
          sres = res.ToString();
        } catch (DivideByZeroException) {
          code = 3;
          sres = "NaN";
        } catch {
          if (SignIsEqual(a, b) && Math.Sign(a) == 1) {
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
        dw.WriteLine($"{a,31} {symbol} {b,31} = {sres,31} | {code}");
      }
    }
    bw.Close();
    dw.Close();
  }
  static void UnaryOpTests(string op) {
    var bw = new BinaryWriter(File.Open($"{tests_path}/{op}.bin", FileMode.Create));
    var dw = new StreamWriter(File.Open($"{tests_path}/{op}.txt", FileMode.Create));
    decimal a, res = decimal.Zero;

    for (int i = 0; i < n_unary_op_cases; ++i) {
      a = unary_cases[i];
      res = decimal.Negate(a);

      bw.Write(a);
      bw.Write(res);
      dw.WriteLine($"{a,31} {op} {res,31}");
    }
    bw.Close();
    dw.Close();
  }
}

public static class ConversionSample {
  public static void print() {
    string path = $"{Tester.tests_path}/conversion_sample.txt";
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

public static class Generator {
  public static int NextInt32(this Random rng) {
    int firstBits = rng.Next(0, 1 << 4) << 28;
    int lastBits = rng.Next(0, 1 << 28);
    return firstBits | lastBits;
  }

  public static decimal NextDecimal(this Random rng) {
    byte scale = (byte)rng.Next(29);
    bool sign = rng.Next(2) == 1;
    return new decimal(rng.NextInt32(), rng.NextInt32(), rng.NextInt32(), sign, scale);
  }

  public static List<decimal> unary_cases = new List<decimal>(Tester.cases);

  public static void AddGeneratedUnaryOpCases() {
    Random rnd = new Random();
    for (int i = 0; i + Tester.cases_n < Tester.n_unary_op_cases; ++i) {
      unary_cases.Add(rnd.NextDecimal());
    }
  }
}

static class SimpleTest {
  public static void mod() {
    var cases = new Tuple<decimal, decimal>[] {
      Tuple.Create(7m, 3m),
      Tuple.Create(-7m, 3m),
      Tuple.Create(7m, -3m),
      Tuple.Create(-7m, -3m),
      Tuple.Create(18446744069414584320m, 000000000000000009m),
      Tuple.Create(-18446744069414584320m, 000000000000000009m),
    };
    decimal a, b;
    for (int i = 0; i < cases.Length; ++i) {
      a = cases[i].Item1;
      b = cases[i].Item2;
      Console.WriteLine($"test: {a} % {b} = {1}", a % b);
    }
  }
}
