begin
  require "socket"
  require "test/unit"
rescue LoadError
end


class TestTCPSocket < Test::Unit::TestCase
  def test_recvfrom # [ruby-dev:24705]
    c = s = nil
    svr = TCPServer.new("localhost", 0)
    th = Thread.new {
      c = svr.accept
      Thread.pass
      ObjectSpace.each_object(String) {|s|
        s.replace "a" if s.length == 0x10000 and !s.frozen?
      }
      c.print("x"*0x1000)
    }
    addr = svr.addr
    sock = TCPSocket.open(addr[2], addr[1])
    assert_raise(RuntimeError, SocketError) {
      sock.recvfrom(0x10000)
    }
  ensure
    th.join
  end
end if defined?(TCPSocket)