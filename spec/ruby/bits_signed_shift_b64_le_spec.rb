# Autogenerated from KST: please remove this line if doing any edits by hand!

RSpec.describe 'BitsSignedShiftB64Le' do
  it 'parses test properly' do
    require 'bits_signed_shift_b64_le'
    r = BitsSignedShiftB64Le.from_file('src/bits_signed_shift_b64_le.bin')

    expect(r.a).to eq 0
    expect(r.b).to eq 255
  end
end
