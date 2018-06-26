
glsl = /home/canowormy/1.1.77.0/x86_64/bin/glslangValidator

incs = 
libs = -L/usr/lib/x86_64-linux-gnu -lvulkan -lglfw

srcs = camera.cc icosahedron.cc main.cc nolight.cc facenormals.cc triangle.cc vkpipe.cc vkstrings.cc vkutils.cc

hdrs = camera.h vkmacros.h vkpipe.h vkstrings.h vkutils.h

objs = $(patsubst %.cc,%o,$(srcs))

%.vert.spv: %.vert
	 $(glsl) -V -o $@ $^

%.frag.spv: %.frag
	 $(glsl) -V -o $@ $^

%.geom.spv: %.geom
	 $(glsl) -V -o $@ $^

%.tesc.spv: %.tesc
	 $(glsl) -V -o $@ $^

%.tese.spv: %.tese
	 $(glsl) -V -o $@ $^

%.o: %.cc $(hdrs)
	g++ -o $@ -Wall -c $<



.PHONY: compile
compile:
	g++ -g -Wall camera.cc icosahedron.cc main.cc nolight.cc facenormals.cc vkdevice.cc vkpipe.cc vkstrings.cc vkutils.cc -L/usr/lib/x86_64-linux-gnu -lglfw -lvulkan
  
