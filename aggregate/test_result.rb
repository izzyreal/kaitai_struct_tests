class TestResult
  class Failure
    attr_reader :file_name
    attr_reader :line_num
    attr_reader :message
    attr_reader :trace
    attr_reader :class_name
    attr_reader :method_name

    def initialize(file_name, line_num, message, trace, class_name, method_name)
      @file_name = file_name
      @line_num = line_num
      @message = message
      @trace = trace
      @class_name = class_name
      @method_name = method_name
    end

    def to_h
      {
        'file_name' => file_name,
        'line_num' => line_num,
        'message' => message,
        'trace' => trace,
        'class_name' => class_name,
        'method_name' => method_name
      }
    end
  end

  attr_reader :name
  attr_reader :status
  attr_reader :elapsed
  attr_reader :failure
  attr_reader :class_name
  attr_reader :method_name

  def initialize(name, status, elapsed, failure, class_name, method_name)
    @name = name
    @status = status
    @elapsed = elapsed
    @failure = failure
    @class_name = class_name
    @method_name = method_name
  end

  def to_h
    h = {
#      'name' => name,
      'status' => status.to_s,
      'elapsed' => elapsed,
      'class_name' => class_name,
      'method_name' => method_name
    }
    h['failure'] = failure.to_h unless failure.nil?
    h
  end
end
