//运行类
// Source code is decompiled from a .class file using FernFlower decompiler (from Intellij IDEA).
public class Main {
   public Main() {
   }

   public static void main(String[] var0) {
      String var1 = "Hello, ZUC!";
      String var2 = ZUC128Util.encrypt(var1);
      System.out.println("密文: " + var2);
      String var3 = ZUC128Util.encryption(var2);
      System.out.println("解密结果: " + var3);
   }
}
