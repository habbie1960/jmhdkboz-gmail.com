begin
  require 'win32ole'
rescue LoadError
end
require "test/unit"

if defined?(WIN32OLE_TYPE)
  class TestWIN32OLE_TYPE < Test::Unit::TestCase

    def test_s_progids
      progids = WIN32OLE_TYPE.progids
      assert_instance_of(Array, progids)
      assert(progids.size > 0)
      assert_instance_of(String, progids[0])
      assert(progids.include?("Shell.Application.1"))
    end

    def test_initialize
      assert_raise(ArgumentError) {
        WIN32OLE_TYPE.new
      }
      assert_raise(ArgumentError) {
        WIN32OLE_TYPE.new("foo")
      }
      assert_raise(WIN32OLERuntimeError) {
        WIN32OLE_TYPE.new("Microsoft Shell Controls And Automation", "foo")
      }
      assert_raise(WIN32OLERuntimeError) {
        WIN32OLE_TYPE.new("Microsoft Shell Controls And Automation", "Application")
      }
      ole_type = WIN32OLE_TYPE.new("Microsoft Shell Controls And Automation", "Shell")
      assert_instance_of(WIN32OLE_TYPE, ole_type)
    end

    def setup
      @ole_type = WIN32OLE_TYPE.new("Microsoft Shell Controls And Automation", "Shell")
    end

    def test_name
      assert_equal("Shell", @ole_type.name)
    end

    def test_ole_type
      assert_equal("Class", @ole_type.ole_type)
    end

    def test_guid
      assert_equal("{13709620-C279-11CE-A49E-444553540000}", @ole_type.guid)
    end

    def test_progid
      assert_equal("Shell.Application.1", @ole_type.progid)
    end

    def test_visible?
      assert(@ole_type.visible?)
      ole_type = WIN32OLE_TYPE.new("Microsoft Shell Controls And Automation", "IShellDispatch")
      assert(!ole_type.visible?)
    end

    def test_to_s
      assert_equal(@ole_type.to_s, @ole_type.name)
    end

    def test_major_version
      assert_equal(0, @ole_type.major_version)
      # ole_type = WIN32OLE_TYPE.new("Microsoft Word 11.0 Object Library", "Documents")
      # assert_equal(8, ole_type.major_version)
    end

    def test_minor_version
      assert_equal(0, @ole_type.minor_version)
      # ole_type = WIN32OLE_TYPE.new("Microsoft Word 11.0 Object Library", "Documents")
      # assert_equal(3, ole_type.minor_version)
    end

    def test_typekind
      assert_equal(5, @ole_type.typekind)
    end

    def test_helpstring
      assert_equal("Shell Object Type Information", @ole_type.helpstring)
    end

    def test_src_type
      ole_type = WIN32OLE_TYPE.new("Microsoft Scripting Runtime", "DriveTypeConst")
      assert_match(/__MIDL___MIDL_itf_scrrun_/, ole_type.src_type)
      assert_equal(nil, @ole_type.src_type)
    end

    def test_helpfile
      assert_equal("", @ole_type.helpfile)
      ole_type = WIN32OLE_TYPE.new("Microsoft Scripting Runtime", "Folders")
      assert_match(/VBENLR98\.CHM$/i, ole_type.helpfile)
    end

    def test_helpcontext
      assert_equal(0, @ole_type.helpcontext)
      ole_type = WIN32OLE_TYPE.new("Microsoft Scripting Runtime", "Folders")
      assert_equal(2181929, ole_type.helpcontext)
    end

    def test_variables
      variables = @ole_type.variables
      assert_instance_of(Array, variables)
      assert(variables.size == 0)

      ole_type = WIN32OLE_TYPE.new("Microsoft Shell Controls And Automation", "ShellSpecialFolderConstants")
      variables = ole_type.variables
      assert_instance_of(Array, variables)
      assert(variables.size > 0)

      assert_instance_of(WIN32OLE_VARIABLE, variables[0])
    end

    def test_ole_methods
      methods = @ole_type.ole_methods
      assert_instance_of(Array, methods)
      assert(methods.size > 0)
      assert_instance_of(WIN32OLE_METHOD, methods[0]);
      assert(methods.collect{|m| m.name}.include?("Application"))
    end

    def test_ole_typelib
      tlib = @ole_type.ole_typelib
      assert_instance_of(WIN32OLE_TYPELIB, tlib)
      assert_equal("Microsoft Shell Controls And Automation", tlib.name)
    end

    def test_implemented_ole_types
      ole_types = @ole_type.implemented_ole_types
      assert_instance_of(Array, ole_types)
      assert(ole_types.size > 0)
      assert_equal("IShellDispatch", ole_types[0].name)
    end

    def test_inspect
      assert_equal("#<WIN32OLE_TYPE:Shell>", @ole_type.inspect)
    end

  end
end
