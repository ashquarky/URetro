.globl injectBAT
injectBAT:
	#Set dbat6u
	lis r3, 0x000000FF@h
	ori r3, r3, 0x000000FF@l
	mtspr 572, r3
	#set dbat6l
	lis r3, 0x30000012@h
	ori r3, r3, 0x30000012@l
	mtspr 573, r3
	#Set ibat6u
	lis r3, 0x000000FF@h
	ori r3, r3, 0x000000FF@l
	mtspr 564, r3
	#set ibat6l
	lis r3, 0x30000012@h
	ori r3, r3, 0x30000012@l
	mtspr 565, r3
	rfi

.globl clearBAT
clearBAT:
	lis r3, 0x00000000@h
	ori r3, r3, 0x00000000@l
	mtspr 572, r3
	
	lis r3, 0x00000000@h
	ori r3, r3, 0x00000000@l
	mtspr 573, r3
	
	lis r3, 0x00000000@h
	ori r3, r3, 0x00000000@l
	mtspr 564, r3
	
	lis r3, 0x00000000@h
	ori r3, r3, 0x00000000@l
	mtspr 565, r3
	
	rfi
	
#Runs the syscall we inject the above code into
.globl RunSyscall36
RunSyscall36:
    mflr r0
    stwu r1, -0x10(r1)
    stw r30, 0x4(r1)
    stw r31, 0x8(r1)
    mr r5, r0
    mr r6, r1
    li r0, 0x3600
    sc
    nop
    mr r0, r5
    mr r1, r6
    lwz r30, 0x04(r1)
    lwz r31, 0x08(r1)
    addi r1, r1, 0x10
    mtlr r0
    blr