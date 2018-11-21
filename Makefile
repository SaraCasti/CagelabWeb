CC=emcc
# A few notes on the options:
#
# -s PRECISE_F32=1 is needed because of a strange bug involving the assignments 
#           of floats. Actually asm.js uses doubles for most computation, 
#           this flags ensure a behavoiur more similar to the expected one.
# --bind    needed for exporting the functions from cpp to js.
# -s DEMANGLE_SUPPORT=1 More consistent naming of the cpp functions is preserved 
#           in asm.js. Useful for cpp stacktrace inside the browser. 
# Reasonable memory options: 
# 
# -s ALLOW_MEMORY_GROWTH=1 
# -s TOTAL_MEMORY=536870912
# -s TOTAL_MEMORY=1073741824 

CFLAGS= -I ./ -s WASM=0 -s TOTAL_MEMORY=1073741824 --memory-init-file 0 -std=c++11 -s PRECISE_F32=1 -s DEMANGLE_SUPPORT=1 --bind  -s LINKABLE=1 -Os -s ASSERTIONS=1 --embed-file meshes/armadillo.obj --embed-file meshes/armadillo_cage.obj


OBJ =  app.bc \
        cagelab_js.bc \
		trimesh/load_save_trimesh.bc \
		coordinates/meanvalue.bc \
		coordinates/green.bc \
		cage/cage.bc

DEPS = coordinates/BarycentricCoordinates.h \
       app.h \
	   coordinates/meanvalue.h \
	   coordinates/green.h \
	   trimesh/trimesh.h \
	   cage/cage.h 

%.bc: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cagelab_asm.js: $(OBJ) $(LIBS)
	$(CC) -o ./generated/$@ $^ $(CFLAGS)

clean:
	rm -f *.bc ./generated/*.js 

test: main.cpp $(OBJ)
	$(CC) -o test/$@ $^ $(CFLAGS)
