# Autogenerated from KST: please remove this line if doing any edits by hand!

require 'enum_import'
require 'enum_0'
require 'enum_deep'

RSpec.describe EnumImport do
  it 'parses test properly' do
    r = EnumImport.from_file('src/enum_0.bin')

    expect(r.pet_1).to eq :animal_cat
    expect(r.pet_2).to eq :animal_hare
  end
end
