APP=النواة.تطبيق

المحول: المحول.c
	gcc -o المحول المحول.c

$(APP): kernel.elf kernel.bin المحول
	@KERNEL_MAIN_ADDR=$$(nm kernel.elf | grep " T KernelMain" | awk '{print "0x"$$1}'); \
	echo "[INFO] KernelMain @ $$KERNEL_MAIN_ADDR"; \
	./المحول "$$KERNEL_MAIN_ADDR"

	@echo ""
	@echo "[✓] اكتمل! انسخ $(APP) إلى EFI/ArabOS/"

clean-المحول:
	rm -f المحول
	rm -f $(APP)
