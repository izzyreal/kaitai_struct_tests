# Autogenerated from KST: please remove this line if doing any edits by hand!

RSpec.describe 'ProcessToUser' do
  it 'parses test properly' do
    require 'process_to_user'
    r = ProcessToUser.from_file('src/process_rotate.bin')

    expect(r.buf1.str).to eq "Hello"
  end
end
