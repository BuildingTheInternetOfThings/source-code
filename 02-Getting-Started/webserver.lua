
wifi.setmode(wifi.STATION)
wifi.sta.config("SSID","PASSWORD")

srv=net.createServer(net.TCP) 
srv:listen(80,function(conn) 
    conn:on("receive",function(conn,payload) 
    print(payload) 
    conn:send("<h1> Hello, NodeMCU.</h1>")
    end) 
end)

