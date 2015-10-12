#include "calculate_parallel.h"
#include "api_par.h"
#include "timer.h"

int get_length_parallel()
{
	return C_M2;
}

int get_length_parallel_x()
{
	return C_M1;
}

int get_length_parallel_y()
{
	return C_M;
}

double* get_sigma_parallel()
{
	double* r = new double[C_M2];
	for (int i = 0; i < C_M2; ++i)
	{
		r[i] = sigma_k1[i];
	}
	return r;
}

double* get_u_parallel()
{
	double* r = new double[C_M2];
	for (int i = 0; i < C_M2; ++i)
	{
		r[i] = u_k1[i];
	}
	return r;
}

double* get_v_parallel()
{
	double* r = new double[C_M2];
	for (int i = 0; i < C_M2; ++i)
	{
		r[i] = v_k1[i];
	}
	return r;
}

double* get_e_parallel()
{
	double* r = new double[C_M2];
	for (int i = 0; i < C_M2; ++i)
	{
		r[i] = e_k1[i];
	}
	return r;
}

double calculate_parallel(bool need_print)
{
	FILE* fout = nullptr;
	FILE* fout_itr = nullptr;
	FILE* fdensity = nullptr;
	FILE* fdensity_new = nullptr;
	FILE* fvelocity = nullptr;
	FILE* ftemperature = nullptr;
	FILE* fpressure = nullptr;
	if (need_print)
	{
		fout = fopen("out_p.txt", "C_w");
		fout_itr = fopen("out_itr_p.txt", "C_w");
		fdensity = fopen("density_p.dat", "C_w");
		fdensity_new = fopen("density-new_p.dat", "C_w");
		fvelocity = fopen("velocity_p.dat", "C_w");
		ftemperature = fopen("temperature_p.dat", "C_w");
		fpressure = fopen("pressure_p.dat", "C_w");
		print_file_header(fout, fdensity, fvelocity, ftemperature, fpressure, fout_itr, C_tau, C_hx, C_hy, C_N);
	}
	const double gamma = 1.4;
	const int time_steps_nbr = 1; // time_steps_nbr - ���������� ����� �� �������
	zeroed_arrays(C_M2, 12);
	set_initial_boundary_conditions(gamma, C_qq, C_w, C_M, C_M1, C_M2, C_Mah2);
	StartTimer();
	for (int current_time_step = 1; current_time_step <= time_steps_nbr; current_time_step++)
	{
		int s_end = 0;
		int s_m = 0;
		int s_e = 0;
		int s_itr;
		prepare_to_iterate(C_M, C_M1, C_qq, C_w, cntr);
		s_itr = interate_over_nonlinearity(gamma, C_qq, C_M, C_M1, C_w, cntr, C_N, C_q, s_m, s_e, s_end);
		if (need_print)
			print_to_file(gamma, s_m, s_e, current_time_step, s_itr, s_end, C_tau, C_hx, C_hy, C_M, C_M1, C_N, C_Mah2, fout, fdensity, fdensity_new, fvelocity, ftemperature, fpressure, fout_itr);
	}
	double time = GetTimer();
	if (need_print)
	{
		print_new_line(fout, fdensity, fvelocity, ftemperature, fpressure);
		close_files(fout, fdensity, fvelocity, ftemperature, fpressure, fout_itr);
	}
	return time;
}
