# Autogenerated from KST: please remove this line if doing any edits by hand!

RSpec.describe 'ParamsPassUsertype' do
  it 'parses test properly' do
    require 'params_pass_usertype'
    r = ParamsPassUsertype.from_file('src/position_in_seq.bin')

    expect(r.first.foo).to eq 1
    expect(r.one.buf).to eq [2].pack('C*')
  end
end
