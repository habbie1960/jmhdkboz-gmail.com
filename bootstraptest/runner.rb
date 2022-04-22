# $Id$

# NOTE:
# Never use optparse in this file.
# Never use test/unit in this file.
# Never use Ruby extensions in this file.

$LOAD_PATH.unshift "#{File.dirname($0)}/lib"
require 'fileutils'

def main
  @ruby = File.expand_path('miniruby')
  @verbose = false
  dir = 'bootstraptest.tmpwd'
  tests = nil
  ARGV.delete_if {|arg|
    case arg
    when /\A--ruby=(.*)/
      @ruby = File.expand_path($1)
      true
    when /\A--sets=(.*)/
      tests = Dir.glob("#{File.dirname($0)}/test_{#{$1}}*.rb")
      puts tests.map {|path| File.basename(path) }.inspect
      true
    when /\A(-v|--v(erbose))\z/
      @verbose = true
    when /\A(-h|--h(elp)?)\z/
      puts(<<-End)
Usage: #{File.basename($0, '.*')} --ruby=PATH [--sets=NAME,NAME,...]
        --sets=NAME,NAME,...        Name of test sets.
    -v, --verbose                   Output test name before exec.
    -h, --help                      Print this message and quit.
End
      exit 0
    else
      false
    end
  }
  if tests and not ARGV.empty?
    $stderr.puts "--tests and arguments are exclusive"
    exit 1
  end
  tests ||= ARGV
  tests = Dir.glob("#{File.dirname($0)}/test_*.rb") if tests.empty?
  pathes = tests.map {|path| File.expand_path(path) }
  in_temporary_working_directory(dir) {
    exec_test pathes
  }
end

def exec_test(pathes)
  @count = 0
  @error = 0
  @errbuf = []
  @location = nil
  pathes.each do |path|
    load File.expand_path(path)
  end
  $stderr.puts
  if @error == 0
    $stderr.puts "PASS #{@count} tests"
    exit 0
  else
    @errbuf.each do |msg|
      $stderr.puts msg
    end
    $stderr.puts "FAIL #{@error}/#{@count} tests failed"
    exit 1
  end
end

def assert_equal(expected, testsrc)
  newtest
  $stderr.puts "\##{@count} #{@location}" if @verbose
  result = get_result_string(testsrc)
  check_coredump
  if expected == result
    $stderr.print '.'
  else
    $stderr.print 'F'
    error pretty(testsrc, expected, result)
  end
rescue Exception => err
  $stderr.print 'E'
  error err.message
end

def pretty(src, ex, result)
  (/\n/ =~ src ? "\n#{adjust_indent(src)}" : src) +
      "  #=> #{result.inspect} (expected #{ex.inspect})"
end

INDENT = 27

def adjust_indent(src)
  untabify(src).gsub(/^ {#{INDENT}}/o, '').gsub(/^/, '   ')
end

def untabify(str)
  str.gsub(/^\t+/) {|tabs| ' ' * (8 * tabs.size) }
end

def get_result_string(src)
  if @ruby
    File.open('bootstraptest.tmp.rb', 'w') {|f|
      f.puts "print(begin; #{src}; end)"
    }
    `#{@ruby} bootstraptest.tmp.rb`
  else
    eval(src).to_s
  end
end

def newtest
  @location = File.basename(caller(2).first)
  @count += 1
  cleanup_coredump
end

def error(msg)
  @errbuf.push "\##{@count} #{@location}: #{msg}"
  @error += 1
end

def in_temporary_working_directory(dir)
  FileUtils.rm_rf dir
  Dir.mkdir dir
  Dir.chdir(dir) {
    yield
  }
end

def cleanup_coredump
  FileUtils.rm_f 'core'
  FileUtils.rm_f Dir.glob('core.*')
end

class CoreDumpError < StandardError; end

def check_coredump
  if File.file?('core') or not Dir.glob('core.*').empty?
    raise CoreDumpError, "core dumped"
  end
end

main
