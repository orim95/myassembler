counter: .data	 5
limit: .data 10
start: lea counter, r1
    mov r1, r2
loopStart: cmp r2, limit 	
    bne continueLoop 
    jmp endLoop 
continueLoop: inc r2         
    prn r2            
    jmp loopStart    

cmp counter, r3
sub #3, r3
clr r4
		bne endLoop
jsr      mayBe
red r5
bne endLoop

mayBe: 	 prn r3
rts


endLoop: stop
