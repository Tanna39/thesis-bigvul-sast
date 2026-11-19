SYSCALL_DEFINE2(osf_sigstack, struct sigstack __user *, uss,
		struct sigstack __user *, uoss)
{
	unsigned long usp = rdusp();
	unsigned long oss_sp = current->sas_ss_sp + current->sas_ss_size;
	unsigned long oss_os = on_sig_stack(usp);
	int error;

	if (uss) {
		void __user *ss_sp;

		error = -EFAULT;
		if (get_user(ss_sp, &uss->ss_sp))
			goto out;

		/* If the current stack was set with sigaltstack, don't
		   swap stacks while we are on it.  */
		error = -EPERM;
		if (current->sas_ss_sp && on_sig_stack(usp))
			goto out;

		/* Since we don't know the extent of the stack, and we don't
		   track onstack-ness, but rather calculate it, we must 
		   presume a size.  Ho hum this interface is lossy.  */
		current->sas_ss_sp = (unsigned long)ss_sp - SIGSTKSZ;
		current->sas_ss_size = SIGSTKSZ;
	}

	if (uoss) {
		error = -EFAULT;
		if (! access_ok(VERIFY_WRITE, uoss, sizeof(*uoss))
		    || __put_user(oss_sp, &uoss->ss_sp)
		    || __put_user(oss_os, &uoss->ss_onstack))
			goto out;
	}

	error = 0;
 out:
	return error;
}
