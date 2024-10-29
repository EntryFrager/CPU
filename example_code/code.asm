push 0
pop rbx
call main
hlt

main:
		in
		pop [rbx + 0]
		in
		pop [rbx + 1]
		in
		pop [rbx + 2]
		push [rbx + 0]
		pop [rbx + 3]
		push [rbx + 1]
		pop [rbx + 4]
		push [rbx + 2]
		pop [rbx + 5]
		push rbx
		push rbx
		push 3
		add
		pop rbx
		call solve_dispatcher
		pop rbx
		push rax
		ret

solve_dispatcher:
		push [rbx + 0]
		push 0					
		jne if_1					
		push [rbx + 1]
		pop [rbx + 3]
		push [rbx + 2]
		pop [rbx + 4]
		push rbx
		push rbx
		push 3
		add
		pop rbx
		call solve_linear
		pop rbx
		push rax
		jmp end_if_0
	if_1:
		push [rbx + 0]
		pop [rbx + 3]
		push [rbx + 1]
		pop [rbx + 4]
		push [rbx + 2]
		pop [rbx + 5]
		push rbx
		push rbx
		push 3
		add
		pop rbx
		call solve_square
		pop rbx
		push rax
	end_if_0:
		push 0
		pop rax
		ret

solve_linear:
		push 0
		push 1
		sub
		pop [rbx + 2]
		push 1
		pop [rbx + 3]
		push 1000
		pop [rbx + 4]
		push [rbx + 0]
		push 0
		jne if_3
		push [rbx + 1]
		push 0
		jne if_5
		push [rbx + 4]
		out
		jmp end_if_4
	if_5:
		push [rbx + 2]
		out
	end_if_4:
		jmp end_if_2
	if_3:
		push 0
		push [rbx + 1]
		push [rbx + 0]
		div
		sub
		pop [rbx + 5]
		push [rbx + 3]
		out
		push [rbx + 5]
		out
	end_if_2:
		push 0
		pop rax
		ret

solve_square:
		push 0
		push 1
		sub
		pop [rbx + 3]
		push 1
		pop [rbx + 4]
		push 2
		pop [rbx + 5]
		push [rbx + 2]
		push 0
		jne if_7
		push 0
		pop [rbx + 6]
		push 0
		push [rbx + 1]
		push [rbx + 0]
		div
		sub
		pop [rbx + 7]
		push [rbx + 5]
		out
		push [rbx + 6]
		out
		push [rbx + 7]
		out
		jmp end_if_6
	if_7:
		push [rbx + 1]
		push [rbx + 1]
		mul
		push 4
		push [rbx + 0]
		mul
		push [rbx + 2]
		mul
		sub
		pop [rbx + 8]
		push [rbx + 8]
		push 0
		jbe if_9
		push [rbx + 8]
		sqrt
		pop [rbx + 9]
		push 0
		push [rbx + 1]
		sub
		push [rbx + 9]
		sub
		push 2
		push [rbx + 0]
		mul
		div
		pop [rbx + 6]
		push 0
		push [rbx + 1]
		sub
		push [rbx + 9]
		add
		push 2
		push [rbx + 0]
		mul
		div
		pop [rbx + 7]
		push [rbx + 5]
		out
		push [rbx + 6]
		out
		push [rbx + 7]
		out
		jmp end_if_8
	if_9:
		push [rbx + 8]
		push 0
		jne if_10
		push 0
		push [rbx + 1]
		push 2
		push [rbx + 0]
		mul
		div
		sub
		pop [rbx + 10]
		push [rbx + 4]
		out
		push [rbx + 10]
		out
		jmp end_if_8
	if_10:
		push [rbx + 3]
		out
	end_if_8:
	end_if_6:
		push 0
		pop rax
		ret
hlt
