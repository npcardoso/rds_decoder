CC=clang
CXX=clang++
RM=rm -f

SRCS=$(shell find . | grep \.cpp$)
OBJS=$(subst .cpp,.o,$(SRCS))
OUT=RDS

all: $(OUT)

$(OUT): $(OBJS)
	$(CXX) -o $(OUT) $(OBJS)

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .depend

include .depend
