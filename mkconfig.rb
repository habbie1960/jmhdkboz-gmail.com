#!./miniruby -s

# avoid warnings with -d.
$install_name ||= nil
$so_name ||= nil

srcdir = File.dirname(__FILE__)
$:.replace [srcdir+"/lib", "."]

require "fileutils"
mkconfig = File.basename($0)

rbconfig_rb = ARGV[0] || 'rbconfig.rb'
unless File.directory?(dir = File.dirname(rbconfig_rb))
  FileUtils.makedirs(dir, :verbose => true)
end

version = RUBY_VERSION
rbconfig_rb_tmp = rbconfig_rb + '.tmp'
config = open(rbconfig_rb_tmp, "w")
$orgout = $stdout.dup
$stdout.reopen(config)

fast = {'prefix'=>TRUE, 'ruby_install_name'=>TRUE, 'INSTALL'=>TRUE, 'EXEEXT'=>TRUE}
print %[
# This file was created by #{mkconfig} when ruby was built.  Any
# changes made to this file will be lost the next time ruby is built.

module Config
  RUBY_VERSION == "#{version}" or
    raise "ruby lib version (#{version}) doesn't match executable version (\#{RUBY_VERSION})"

]

v_fast = []
v_others = []
has_version = false
File.foreach "config.status" do |line|
  next if /^#/ =~ line
  if /^s[%,]@program_transform_name@[%,]s,(.*)/ =~ line
    next if $install_name
    ptn = $1.sub(/\$\$/, '$').split(/,/)	#'
    v_fast << "  CONFIG[\"ruby_install_name\"] = \"" + "ruby".sub(/#{ptn[0]}/,ptn[1]) + "\"\n"
  elsif /^s[%,]@(\w+)@[%,](.*)[%,]/ =~ line
    name = $1
    val = $2 || ""
    next if /^(?:ac_.*|DEFS|configure_input)$/ =~ name
    next if /^\$\(ac_\w+\)$/ =~ val
    next if /^\$\{ac_\w+\}$/ =~ val
    next if /^\$ac_\w+$/ =~ val
    next if $install_name and /^RUBY_INSTALL_NAME$/ =~ name
    next if $so_name and /^RUBY_SO_NAME$/ =~  name
    v = "  CONFIG[\"" + name + "\"] = " +
      val.gsub(/\$(?:\$|\{?(\w+)\}?)/) {$1 ? "$(#{$1})" : $&}.dump + "\n"
    if fast[name]
      v_fast << v
    else
      v_others << v
    end
    has_version = true if name == "MAJOR"
  elsif /^(?:ac_given_)?INSTALL=(.*)/ =~ line
    v_fast << "  CONFIG[\"INSTALL\"] = " + $1 + "\n"
  end
#  break if /^CEOF/
end

v_fast.collect! do |x|
  if /"prefix"/ === x
    x.sub(/= (.*)/, '= (TOPDIR || DESTDIR + \1)')
  else
    x
  end
end

drive = File::PATH_SEPARATOR == ';'

prefix = '/lib/ruby/' + RUBY_VERSION.sub(/\.\d+$/, '') + '/' + RUBY_PLATFORM
print "  TOPDIR = File.dirname(__FILE__).chomp!(#{prefix.dump})\n"
print "  DESTDIR = ", (drive ? "TOPDIR && TOPDIR[/\\A[a-z]:/i] || " : ""), "'' unless defined? DESTDIR\n"
print "  CONFIG = {}\n"
print "  CONFIG[\"DESTDIR\"] = DESTDIR\n"

unless has_version
  RUBY_VERSION.scan(/(\d+)\.(\d+)\.(\d+)/) {
    print "  CONFIG[\"MAJOR\"] = \"" + $1 + "\"\n"
    print "  CONFIG[\"MINOR\"] = \"" + $2 + "\"\n"
    print "  CONFIG[\"TEENY\"] = \"" + $3 + "\"\n"
  }
end

dest = drive ? /= \"(?!\$[\(\{])(?:[a-z]:)?/i : /= \"(?!\$[\(\{])/
v_others.collect! do |x|
  if /^\s*CONFIG\["(?!abs_|old)[a-z]+(?:_prefix|dir)"\]/ === x
    x.sub(dest, '= "$(DESTDIR)')
  else
    x
  end
end

if $install_name
  v_fast << "  CONFIG[\"ruby_install_name\"] = \"" + $install_name + "\"\n"
  v_fast << "  CONFIG[\"RUBY_INSTALL_NAME\"] = \"" + $install_name + "\"\n"
end
if $so_name
  v_fast << "  CONFIG[\"RUBY_SO_NAME\"] = \"" + $so_name + "\"\n"
end

print v_fast, v_others
print <<EOS
  CONFIG["ruby_version"] = "$(MAJOR).$(MINOR)"
  CONFIG["rubylibdir"] = "$(libdir)/ruby/$(ruby_version)"
  CONFIG["archdir"] = "$(rubylibdir)/$(arch)"
  CONFIG["sitelibdir"] = "$(sitedir)/$(ruby_version)"
  CONFIG["sitearchdir"] = "$(sitelibdir)/$(sitearch)"
  CONFIG["topdir"] = File.dirname(__FILE__)
  MAKEFILE_CONFIG = {}
  CONFIG.each{|k,v| MAKEFILE_CONFIG[k] = v.dup}
  def Config::expand(val, config = CONFIG)
    val.gsub!(/\\$\\$|\\$\\(([^()]+)\\)|\\$\\{([^{}]+)\\}/) do |var|
      if !(v = $1 || $2)
	'$'
      elsif key = config[v]
	config[v] = false
	Config::expand(key, config)
	config[v] = key
      else
	var
      end
    end
    val
  end
  CONFIG.each_value do |val|
    Config::expand(val)
  end
end
CROSS_COMPILING = nil unless defined? CROSS_COMPILING
EOS
$stdout.flush
$stdout.reopen($orgout)
config.close
if $timestamp and
   File.exist?(rbconfig_rb) and
   FileUtils.compare_file(rbconfig_rb, rbconfig_rb_tmp)
  puts "#{rbconfig_rb} unchanged"
  File.unlink(rbconfig_rb_tmp)
else
  puts "#{rbconfig_rb} updated"
  File.rename(rbconfig_rb_tmp, rbconfig_rb)
end
if String === $timestamp
  FileUtils.touch($timestamp)
end

# vi:set sw=2:
