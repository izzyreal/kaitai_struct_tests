# Autogenerated from KST: please remove this line if doing any edits by hand!

RSpec.describe 'ParamsPassArrayInt' do
  it 'parses test properly' do
    require 'params_pass_array_int'
    r = ParamsPassArrayInt.from_file('src/position_to_end.bin')

    expect(r.pass_ints.nums.length).to eq 3
    expect(r.pass_ints.nums[0]).to eq 513
    expect(r.pass_ints.nums[1]).to eq 1027
    expect(r.pass_ints.nums[2]).to eq 1541
    expect(r.pass_ints_calc.nums.length).to eq 2
    expect(r.pass_ints_calc.nums[0]).to eq 27643
    expect(r.pass_ints_calc.nums[1]).to eq 7
  end
end
