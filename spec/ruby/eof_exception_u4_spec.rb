# Autogenerated from KST: please remove this line if doing any edits by hand!

RSpec.describe 'EofExceptionU4' do
  it 'parses test properly' do
    require 'eof_exception_u4'
    expect {
      r = EofExceptionU4.from_file('src/term_strz.bin')
    }.to raise_error(EOFError)
  end
end
