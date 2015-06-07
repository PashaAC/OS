all:
	$(MAKE) -C lib/
	$(MAKE) -C cat/
	$(MAKE) -C revwords/
	$(MAKE) -C filter/
	$(MAKE) -C bufcat/
	$(MAKE) -C foreach/
	$(MAKE) -C simplesh/
	
clean: 
	$(MAKE) clean -C lib/
	$(MAKE) clean -C cat/
	$(MAKE) clean -C revwords/
	$(MAKE) clean -C filter/
	$(MAKE) clean -C bufcat/
	$(MAKE) clean -C foreach/
	$(MAKE) clean -C simplesh/

