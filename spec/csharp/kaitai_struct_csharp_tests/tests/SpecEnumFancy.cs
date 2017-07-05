using NUnit.Framework;

namespace Kaitai
{
    [TestFixture]
    public class SpecEnumFancy : CommonSpec
    {
        [Test]
        public void TestEnumFancy()
        {
            var r = EnumFancy.FromFile(SourceFile("enum_0.bin"));
            Assert.AreEqual(r.Pet1, EnumFancy.Animal.Cat);
            Assert.AreEqual(r.Pet2, EnumFancy.Animal.Dog);
        }
    }
}
