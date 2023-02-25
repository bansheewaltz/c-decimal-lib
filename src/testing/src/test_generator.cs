using System;
using System.IO;
using System.Globalization;
using System.Linq;
using System.Collections.Generic;
using System.Diagnostics;

using static Generator;
using CustomExtensions;
using CustomUtilities;
using static CustomUtilities.Headers;
using static CustomUtilities.If;

static class Tester {
  public static string cases_path = "testing/cases";
  public static string cases_txt_path = $"{cases_path}/txt";
  public static string cases_bin_path = $"{cases_path}/bin";
  static void Main() {
    /* handy stuff */
    // SimpleTest.signed_zero_multiply();
    // Directory.CreateDirectory(cases_txt_path);
    // ConversionSample.print();

    Directory.CreateDirectory(cases_bin_path);
    /* arithmetic operations */
    Tester.CommutativeBinaryOpResults("add");
    Tester.CommutativeBinaryOpResults("mul");
    Tester.NCommutativeBinaryOpResults("sub");
    Tester.NCommutativeBinaryOpResults("div");
    Tester.NCommutativeBinaryOpResults("mod");
    /* comparison operations */
    Tester.ComparisonOpResults("is_less");
    Tester.ComparisonOpResults("is_less_or_equal");
    Tester.ComparisonOpResults("is_greater");
    Tester.ComparisonOpResults("is_greater_or_equal");
    Tester.ComparisonOpResults("is_equal");
    Tester.ComparisonOpResults("is_not_equal");
    // /* convertors and parsers */
    {
      unary_op_cases_n = 200;
      decimal_cases = Generator.GenerateDecimalCases();
      int_cases = Generator.GenerateIntCases();
      float_cases = Generator.GenerateFloatCases();
    }
    Tester.ConvertOpResults("from_int_to_decimal");
    Tester.ConvertOpResults("from_float_to_decimal");
    Tester.ConvertOpResults("from_decimal_to_int");
    Tester.ConvertOpResults("from_decimal_to_float");
    /* other functions */
    Tester.UnaryOpResults("negate");
    Tester.UnaryOpResults("truncate");
    Tester.UnaryOpResults("round");
    Tester.UnaryOpResults("floor");
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
    -5m / 11m,
    5m / 11m,
    -15m / 13m,
    15m / 13m,
    -1.0000000000000000000000000000m,
    1.0000000000000000000000000000m,
    -0.0000000000000000000000000001M,
    0.0000000000000000000000000001M,
    -7.9228162514264337593543950335M,
    +7.9228162514264337593543950335M,
    decimal.MinValue,
    decimal.MaxValue,
  };
  public static int unary_op_cases_n;
  public static int cases_n = cases.Count;
  public static List<Decimal> decimal_cases;
  public static List<Single> float_cases;
  public static List<Int32> int_cases;
  static void CommutativeBinaryOpResults(string op) {
    string path = $"{cases_txt_path}/arithmetics/";
    Directory.CreateDirectory(path);
    var bw = new BinaryWriter(File.Open($"{cases_bin_path}/{op}.bin", FileMode.Create));
    var dw = new StreamWriter(File.Open($"{path}/{op}.txt", FileMode.Create));
    Headers.WriteBinaryOpHeader(dw);
    decimal a, b, res = 0, res_norm = 0;
    string sres, symbol = "?";
    Int32 ret;
    int n = 0;

    for (int i = 0; i < cases_n; ++i) {
      for (int j = i; j < cases_n; ++j, n++) {
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
              res = Decimal.Multiply(a, b);
              break;
            default:
              Terminate.PrintError(Terminate.GetCurrentMethod(), op);
              break;
          }
          ret = 0;
          sres = res.ToString();
          res_norm = res.NormalizeBits();
        } catch {
          if ((op == "mul" && If.SignsAreEqual(a, b)) ||
              (op == "add" && If.SignsAreEqual(a, b) && Math.Sign(a) == 1)) {
            ret = 1;
            sres = "+inf";
          } else {
            ret = 2;
            sres = "-inf";
          }
        }
        bw.Serialize(a, b, ret, res_norm);

        var hex_norm = res_norm.GetHexString(ret);
        var hex = res.GetHexString(ret);
        dw.WriteLine(
            $"{n,3}|{a,32} {symbol} {b,31} = {sres,31} | {ret} | {hex_norm,-35} | {hex,-35} = {a.GetHexString()} {symbol} {b.GetHexString()}");
      }
    }
    bw.Close();
    dw.Close();
  }
  static void NCommutativeBinaryOpResults(string op) {
    string path = $"{cases_txt_path}/arithmetics/";
    Directory.CreateDirectory(path);
    var bw = new BinaryWriter(File.Open($"{cases_bin_path}/{op}.bin", FileMode.Create));
    var dw = new StreamWriter(File.Open($"{path}/{op}.txt", FileMode.Create));
    Headers.WriteBinaryOpHeader(dw);
    decimal a, b, res = 0, res_norm = 0;
    string sres, symbol = "?";
    Int32 ret;
    int n = 0;

    for (int i = 0; i < cases_n; ++i) {
      for (int j = 0; j < cases_n; ++j, ++n) {
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
              {
                if (Math.Abs(a) == Decimal.MaxValue && Math.Abs(b) < 1m) {
                  res = 0;
                }
                if (Math.Abs(a) == Decimal.MaxValue && Math.Abs(b) == 1m) {
                  res = 0;
                }
              }
              break;
            default:
              Terminate.PrintError(Terminate.GetCurrentMethod(), op);
              break;
          }
          ret = 0;
          sres = res.ToString();
          res_norm = res.NormalizeBits();
        } catch (DivideByZeroException) {
          ret = 3;
          sres = "NaN";
        } catch {
          if (op == "sub" && !SignsAreEqual(a, b) && Math.Sign(b) == -1 ||  //
              op != "sub" && SignsAreEqual(a, b)) {
            ret = 1;
            sres = "+inf";
          } else {
            ret = 2;
            sres = "-inf";
          }
        }
        bw.Serialize(a, b, ret, res_norm);

        var hex_norm = res_norm.GetHexString(ret);
        var hex = res.GetHexString(ret);
        dw.WriteLine(
            $"{n,3}|{a,32} {symbol} {b,31} = {sres,31} | {ret} | {hex_norm,-35} | {hex,-35} = {a.GetHexString()} {symbol} {b.GetHexString()}");
      }
    }
    bw.Close();
    dw.Close();
  }
  static void ComparisonOpResults(string op) {
    string path = $"{cases_txt_path}/comparisons/";
    Directory.CreateDirectory(path);
    var bw = new BinaryWriter(File.Open($"{cases_bin_path}/{op}.bin", FileMode.Create));
    var dw = new StreamWriter(File.Open($"{path}/{op}.txt", FileMode.Create));
    Headers.WriteComparisonOpHeader(dw);
    decimal a, b;
    string symbol = "?";
    Int32 ret = 0;
    int n = 0;

    for (int i = 0; i < cases_n; ++i) {
      for (int j = 0; j < cases_n; ++j, ++n) {
        a = cases[i];
        b = cases[j];

        switch (op) {
          case "is_less":
            symbol = "<";
            ret = a < b ? 1 : 0;
            break;
          case "is_less_or_equal":
            symbol = "<=";
            ret = a <= b ? 1 : 0;
            break;
          case "is_greater":
            symbol = ">";
            ret = a > b ? 1 : 0;
            break;
          case "is_greater_or_equal":
            symbol = ">=";
            ret = a >= b ? 1 : 0;
            break;
          case "is_equal":
            symbol = "==";
            ret = a == b ? 1 : 0;
            break;
          case "is_not_equal":
            symbol = "!=";
            ret = a != b ? 1 : 0;
            break;
          default:
            Terminate.PrintError(Terminate.GetCurrentMethod(), op);
            break;
        }
        bw.Serialize(a, b, ret);
        dw.WriteLine(
            $"{n,3}|{a,31} {symbol,2} {b,31} | {ret} | {a.GetHexString()} | {b.GetHexString()}");
      }
    }
    bw.Close();
    dw.Close();
  }
  static void UnaryOpResults(string op) {
    string path = $"{cases_txt_path}/others/";
    Directory.CreateDirectory(path);
    var bw = new BinaryWriter(File.Open($"{cases_bin_path}/{op}.bin", FileMode.Create));
    var dw = new StreamWriter(File.Open($"{path}/{op}.txt", FileMode.Create));
    Headers.WriteOthersOpHeader(dw);
    decimal a, res = decimal.Zero;
    Int32 ret;
    int n = 0;

    for (int i = 0; i < unary_op_cases_n; ++i, ++n) {
      a = decimal_cases[i];
      switch (op) {
        case "negate":
          res = decimal.Negate(a);
          break;
        case "truncate":
          res = decimal.Truncate(a);
          break;
        case "round":
          res = decimal.Round(a);
          break;
        case "floor":
          res = decimal.Floor(a);
          break;
        default:
          Terminate.PrintError(Terminate.GetCurrentMethod(), op);
          break;
      }
      ret = 0;
      bw.Serialize(a, ret, res);
      dw.WriteLine($"{n,3}|{a,32}|{res,32}|{ret,2} | {a.GetHexString()} -> {res.GetHexString()}");
    }
    bw.Close();
    dw.Close();
  }
  static void ConvertOpResults(string op) {
    string path = $"{cases_txt_path}/convertors/";
    Directory.CreateDirectory(path);
    var bw = new BinaryWriter(File.Open($"{cases_bin_path}/{op}.bin", FileMode.Create));
    var dw = new StreamWriter(File.Open($"{path}/{op}.txt", FileMode.Create));
    Headers.WriteConvertorOpHeader(dw);
    Int32 in_int = 0, res_int = 0;
    Single in_float = 0, res_float = 0;
    Decimal in_dec = 0, res_dec = 0;
    String in_str = "placeholder", res_str = "placeholder";
    int n = 0, left_field = 0, right_field = 0;
    Int32 ret = 0;

    for (int i = 0; i < unary_op_cases_n; ++i, ++n) {
      ret = 0;
      switch (op) {
        case "from_int_to_decimal":
          in_int = int_cases[i];
          in_str = Convert.ToString(in_int);
          try {
            res_dec = (decimal)(in_int);
          } catch {
            ret = 1;
            res_str = "error";
          }
          res_str = Convert.ToString(res_dec);
          left_field = 12;
          right_field = 12;

          bw.Write(in_int);
          bw.Write(res_dec);
          break;
        case "from_float_to_decimal":
          in_float = float_cases[i];
          in_str = Convert.ToString(in_float);
          try {
            res_dec = ((decimal)(in_float)).NormalizeBits();
          } catch {
            ret = 1;
            res_str = "error";
          }
          float abs = Math.Abs(in_float);
          if (0 != abs && abs < 1e-28) {
            ret = 1;
            res_dec = 0m;
          }
          res_str = Convert.ToString(res_dec);
          left_field = 14;
          right_field = 32;

          bw.Write(in_float);
          bw.Write(res_dec);
          break;
        case "from_decimal_to_int":
          in_dec = decimal_cases[i];
          in_str = Convert.ToString(in_dec);
          try {
            res_int = (int)(in_dec);
            res_str = Convert.ToString(res_int);
          } catch {
            ret = 1;
            res_str = "error";
          }
          left_field = 32;
          right_field = 12;

          bw.Write(in_dec);
          bw.Write(res_int);
          break;
        case "from_decimal_to_float":
          in_dec = decimal_cases[i];
          in_str = Convert.ToString(in_dec);
          try {
            res_float = (float)(in_dec);
          } catch {
            ret = 1;
            res_str = "error";
          }
          res_str = Convert.ToString(res_float);
          left_field = 32;
          right_field = 14;

          bw.Write(in_dec);
          bw.Write(res_float);
          break;
        default:
          Terminate.PrintError(Terminate.GetCurrentMethod(), op);
          break;
      }
      bw.Write(ret);
      var fmt = $"{n, 3}|{{0, {left_field}}} |{{1, {right_field}}} |{ret,2} | ";
      dw.Write(fmt, in_str, res_str);

      switch (op) {
        case "from_int_to_decimal":
          dw.WriteLine($"{in_int.GetHexString()} -> {res_dec.GetHexString(ret)}");
          break;
        case "from_float_to_decimal":
          dw.WriteLine($"{in_float.GetHexString()} -> {res_dec.GetHexString(ret)}");
          break;
        case "from_decimal_to_float":
          dw.WriteLine($"{in_dec.GetHexString()} -> {res_float.GetHexString()}");
          break;
        case "from_decimal_to_int":
          dw.WriteLine($"{in_dec.GetHexString()} -> {res_int.GetHexString()}");
          break;
      }
    }
    bw.Close();
    dw.Close();
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
  public static float NextFloat(this Random random) {
    var buffer = new byte[4];
    random.NextBytes(buffer);
    return BitConverter.ToSingle(buffer, 0);
  }

  public static Random rnd = new Random();
  public static List<decimal> GenerateDecimalCases() {
    var decimal_cases = new List<decimal>(Tester.cases);
    for (int i = Tester.cases_n; i < Tester.unary_op_cases_n; ++i) {
      decimal_cases.Add(rnd.NextDecimal());
    }
    return decimal_cases;
  }
  public static List<Int32> GenerateIntCases() {
    var int_cases = new List<Int32>();
    int_cases.Add(0);
    int_cases.Add(1);
    int_cases.Add(-1);
    int_cases.Add(int.MinValue);
    int_cases.Add(int.MaxValue);
    for (int i = 0; i < Tester.unary_op_cases_n; ++i) {
      int_cases.Add(rnd.NextInt32());
    }
    return int_cases;
  }
  public static List<Single> GenerateFloatCases() {
    var float_cases = new List<Single>();
    float_cases.Add(0f);
    float_cases.Add(0.1f);
    float_cases.Add(-0.1f);
    float_cases.Add(0.0001f);
    float_cases.Add(-0.0001f);
    float_cases.Add(1f);
    float_cases.Add(-1f);
    float_cases.Add(Single.Epsilon);
    float_cases.Add(Single.NegativeInfinity);
    float_cases.Add(Single.PositiveInfinity);
    float_cases.Add(Single.MinValue);
    float_cases.Add(Single.MaxValue);
    float_cases.Add(Single.NaN);
    // float_cases.Add(Single.NegativeZero);
    // float_cases.Add(Single.E);
    // float_cases.Add(Single.Tau);
    for (int i = 0; i < Tester.unary_op_cases_n; ++i) {
      float_cases.Add(rnd.NextFloat());
    }
    return float_cases;
  }
}

namespace CustomExtensions {
  public static class DecimalExtensions {
    public static decimal Normalize(this decimal value) {
      return value / 1.000000000000000000000000000000000m;
    }
    public static decimal NormalizeBits(this decimal d) {
      decimal tmp = decimal.Parse(d.Normalize().ToString());

      if (d != 0) {
        return tmp;
      } else {
        int[] parts = Decimal.GetBits(d);
        bool sign = (parts[3] & 0x80000000) != 0;

        if (sign == true) {
          return Decimal.Multiply(tmp, -1);
        } else {
          return tmp;
        }
      }
    }
  }
  public static class BinaryWriterExtensions {
    public static void Serialize(this BinaryWriter bw, decimal a, decimal b, Int32 ret,
                                 decimal res) {
      bw.Write(a);
      bw.Write(b);
      bw.Write(ret);
      bw.Write(res);
    }
    public static void Serialize(this BinaryWriter bw, decimal a, decimal b, Int32 ret) {
      bw.Write(a);
      bw.Write(b);
      bw.Write(ret);
    }
    public static void Serialize(this BinaryWriter bw, decimal a, Int32 ret) {
      bw.Write(a);
      bw.Write(ret);
    }
    public static void Serialize(this BinaryWriter bw, decimal a, Int32 ret, decimal res) {
      bw.Write(a);
      bw.Write(ret);
      bw.Write(res);
    }
  }
  public static class StringExtensions {
    public static string Repeat(this char c, int n) {
      return new String(c, n);
    }
    public static string GetHexString(this decimal d) {
      var bits = decimal.GetBits(d);
      return $"{bits[3],8:X8}{bits[2],9:X8}{bits[1],9:X8}{bits[0],9:X8}";
    }
    public static string GetHexString(this decimal d, Int32 ret) {
      return ret == 0 ? d.GetHexString() : "undefined behaviour";
    }
    public static string GetHexString(this int i) {
      return i.ToString("X8");
    }
    public static string GetHexString(this Single s) {
      var bytes = BitConverter.GetBytes(s);
      return BitConverter.ToInt32(bytes, 0).GetHexString();
    }
    public static string GetBinaryString(this decimal d) {
      var bits = decimal.GetBits(d);
      var binary_string = string.Empty;

      for (int i = 3; i >= 0; --i) {
        if (i == 3) {
          binary_string += $"{Convert.ToString(bits[i], 2).PadLeft(32, '0')}";
          continue;
        }
        binary_string += $"{Convert.ToString(bits[i], 2).PadLeft(32, '0'), 33}";
      }

      return binary_string;
    }
  }
}

namespace CustomUtilities {
  public static class Terminate {
    public static string GetCurrentMethod() {
      var st = new StackTrace();
      var sf = st.GetFrame(1);

      return sf.GetMethod().Name;
    }
    public static void PrintError(string method_name, string op) {
      Console.WriteLine($"Incorrect function \"{method_name}\" input: \"{op}\"");
      string[] files =
          Directory.GetFiles(Tester.cases_bin_path, $"*{op}.*", SearchOption.AllDirectories);
      for (int i = 0; i < files.Length; ++i) {
        File.Delete(files[i]);
      }
      System.Environment.Exit(1);
    }
  }
  public static class Headers {
    public static void WriteBinaryOpHeader(StreamWriter dw) {
      dw.WriteLine(
          $" n |{"a",16}{"|",18}{"b", 17}{"|",17}{"result", 19}{"|",15}{"ret", 3}|{"normalized result in hex",30}{"|",8}{"result in hex",25}{"|",13}{"a in hex",23}{"|",15}{"b in hex",23}");
      dw.WriteLine('-'.Repeat(260));
    }
    public static void WriteComparisonOpHeader(StreamWriter dw) {
      dw.WriteLine(
          $"{" n "}|{"a",16}{"|",16}op|{"b", 17}{"|",16}{"ret", 3}|{"a in hex",23}{"|",15}{"b in hex",23}");
      dw.WriteLine('-'.Repeat(150));
    }
    public static void WriteOthersOpHeader(StreamWriter dw) {
      dw.WriteLine(
          $"{" n "}|{"argument",20}{"|",13}{"result", 19}{"|",14}{"ret", 3}|{"argument in hex",26}{"result in hex",36}");
      dw.WriteLine('-'.Repeat(149));
    }
    public static void WriteConvertorOpHeader(StreamWriter dw) {
      dw.WriteLine(" n |");
      dw.WriteLine('-'.Repeat(106));
    }
  }
  public static class If {
    public static bool SignsAreEqual(decimal a, decimal b) {
      return Math.Sign(a) == Math.Sign(b);
    }
  }
}

public static class ConversionSample {
  public static void print() {
    string path = $"{Tester.cases_txt_path}/conversion_sample.txt";
    var sw = new StreamWriter(File.Open(path, FileMode.Create));

    Decimal[] values = { Decimal.Zero,
                         0.0000000000000000000000000000M,
                         1M,
                         -1M,
                         0.1M,
                         -0.1M,
                         1M / 9M,
                         -1M / 9M,
                         1M,
                         0.0000000000000000000000000001M,
                         -0.0000000000000000000000000001M,
                         5M / 11M,
                         -5M / 11M,
                         0.5M,
                         -0.5M,
                         1M,
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
                         15M / 13M,
                         -15M / 13M,
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

    var f1 = '-'.Repeat(20);
    var f2 = '-'.Repeat(8);
    var f3 = '-'.Repeat(32);
    sw.WriteLine("{0,31}  {1,8} {2,8} {3,8} {4,8}  {1,-32} {2,-32} {3,-32} {4,-32}  {0,-32}",
                 "Decimal type number", "Bits[3]", "Bits[2]", "Bits[1]", "Bits[0]");
    sw.WriteLine(
        $"{f1,31}  {f2,8} {f2,8} {f2,8} {f2,8}  {f3,-32} {f3,-32} {f3,-32} {f3,-32}  {f1,-32}");
    foreach (var value in values) {
      sw.WriteLine($"{value,31}  {value.GetHexString()}  {value.GetBinaryString()}  {value,31}");
    }

    sw.WriteLine("{0}{1}", ' '.Repeat(70), "|      |<------><-------------->");
    sw.WriteLine("{0}{1}", ' '.Repeat(70), "|      [exponent]  [not used]");
    sw.WriteLine("{0}{1}", ' '.Repeat(66), "[sign bit]");
    sw.WriteLine("{0}{1}", ' '.Repeat(71), "|<---->|");
    sw.WriteLine("{0}{1}", ' '.Repeat(70), "[not used]");

    sw.Close();
  }
}

static class SimpleTest {
  public static void mod() {
    var cases = new Tuple<decimal, decimal>[] {
      Tuple.Create(7m, 3m),
      Tuple.Create(-7m, 3m),
      Tuple.Create(7m, -3m),
      Tuple.Create(-7m, -3m),
      Tuple.Create(18446744069414584320m, 0.00000000000000009m),
      Tuple.Create(-18446744069414584320m, 0.00000000000000009m),
    };
    decimal a, b;
    for (int i = 0; i < cases.Length; ++i) {
      a = cases[i].Item1;
      b = cases[i].Item2;
      Console.WriteLine($"test: {a} % {b} = {a % b}");
    }
  }
  public static void normalization() {
    decimal[] cases = { 0m, Decimal.Add(-1m, 1.0m), 0.1m, 0.10m, 0.100m, 0.1000000m };
    decimal d;

    for (int i = 0; i < cases.Length; ++i) {
      d = cases[i];
      Console.WriteLine($"{d,32}  {d.GetHexString()}  {d.GetBinaryString()}");
    }

    Console.WriteLine("Normalized output");
    for (int i = 0; i < cases.Length; ++i) {
      d = cases[i];
      Console.WriteLine($"{d.Normalize(),32}  {d.GetHexString()}  {d.GetBinaryString()}");
    }

    Console.WriteLine("Normalized bit strings");
    for (int i = 0; i < cases.Length; ++i) {
      d = cases[i].NormalizeBits();
      Console.WriteLine($"{d,32}  {d.GetHexString()}  {d.GetBinaryString()}");
    }
  }
  public static void check_difference() {
    decimal d = -0.1m * -7.9228162514264337593543950335m;
    Console.WriteLine($"{d} {d.GetHexString()}");

    decimal a = new decimal(-1717986918, 1717986919, 429496729, false, 28);
    decimal b = new decimal(-1717986919, 1717986919, 429496729, false, 28);
    Console.WriteLine($"{a} {a.GetHexString()}");
    Console.WriteLine($"{b} {b.GetHexString()}");
  }
  public static void signed_zero() {
    decimal pz = new decimal(0, 0, 0, false, 0);
    decimal nz = new decimal(0, 0, 0, true, 0);
    Console.WriteLine($"{pz} {pz.GetHexString()}");
    Console.WriteLine($"{nz} {nz.GetHexString()}");

    decimal b = Decimal.Multiply(0m, -1);
    Console.WriteLine($"{b} {b.GetHexString()}");
    Console.WriteLine(Math.Sign(b) + " " + Math.Sign(-1m));
  }
  public static void float_to_dec() {
    float a = 2.703282E-16f;
    decimal b = (decimal)a;
    Console.WriteLine(System.Runtime.InteropServices.Marshal.SizeOf(a));
    Console.WriteLine(System.Runtime.InteropServices.Marshal.SizeOf(b));
    Console.WriteLine($"{a} = {b}");
  }
  public static void signed_zero_multiply() {
    var cases = new Tuple<decimal, decimal>[] {
      Tuple.Create(new decimal(0, 0, 0, false, 0), new decimal(0, 0, 0, true, 0)),
      Tuple.Create(0m, -1m),
      Tuple.Create(1m, -1m),
      Tuple.Create(1m, -0.1m),
      Tuple.Create(0.1m, -0.1m),
      Tuple.Create(-0.1m, -0.1m),
      Tuple.Create(0m, -0.3333333333333333333333333333m),
      Tuple.Create(0m, -0.33333333333m),
      Tuple.Create(0m, -0.11111111111m),
      Tuple.Create(0m, -0.00000000001m),
      Tuple.Create(0m, -0.0000000000000000000000000001m),
      Tuple.Create(0m, -7.922816251m),
      Tuple.Create(0m, -7.92281625m),
      Tuple.Create(0m, -15 / 13m),
      Tuple.Create(0m, -15.92281625m),
      Tuple.Create(0m, -15.922816983m),
    };
    decimal a, b;
    for (int i = 0; i < cases.Length; ++i) {
      a = cases[i].Item1;
      b = cases[i].Item2;
      Console.WriteLine(
          $"{a,32} * {b,32} = {Decimal.Multiply(a, b).GetHexString()} = {a.GetHexString()} * {b.GetHexString()}");
    }
  }
}
