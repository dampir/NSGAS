#include <stdio.h>
#include "seq_calculate.h"
#include "seq_trajectory_cone.h"
#include "seq_continuity_sigma.h"
#include "seq_motion.h"
#include "seq_energy_epsilon.h"
#include "seq_api.h"
#include "timer.h"

FILE *out, *density, *density_new, *velocity, *temperature, *pressure, *out_itr;

void clear_memory(const int array_element_count)
{
	for (int i = 0; i < 2 * array_element_count; i++)
	{
		delete[] A[i];
	}
	delete[] A;
	delete[] B;
	delete[] D;
	delete[] f;
	delete[] Sigma_k;
	delete[] e_k;
	delete[] e_k_mu;
	delete[] e_k1;
	delete[] e_kk;
	delete[] v_kk;
	delete[] e2;
	delete[] T;
	delete[] Sigma_kk;
	delete[] u_k;
	delete[] u_kk;
	delete[] v_k;
	delete[] Sigma_k1;
	delete[] u_k1;
	delete[] v_k1;
	delete[] u2;
	delete[] v2;
	delete[] SigmaX_k;
	delete[] uX_k;
	delete[] vY_k;
	delete[] eR_k;
}

int get_length()
{
	return M2;
}

int get_length_x()
{
	return M1;
}

int get_length_y()
{
	return M;
}

double* get_sigma()
{
	return Sigma_k1;
}

double* get_u()
{	
	return u_k1;
}

double* get_v()
{
	return v_k1;
}

double* get_e()
{
	return e_k1;
}

double calculate(bool need_print)
{
	int i = 0;
	int j = 0;
	int a;
	int d;

	if (need_print)
	{
		out = fopen("out.txt", "w");
		density = fopen("density.dat", "w");
		density_new = fopen("density-new.dat", "w");
		velocity = fopen("velocity.dat", "w");
		temperature = fopen("temperature.dat", "w");
		pressure = fopen("pressure.dat", "w");
		out_itr = fopen("out_itr.txt", "w");

		fprintf(out, "Cone_2D\n\n");
		fprintf(out, "N = %i\t hx = %.5f\t hy = %.5f\t tau = %.5f\n\n", N, hx, hy, tau);

		fprintf(out_itr, "Cone_2D\n\n");
		fprintf(out_itr, "N = %i\t hx = %.5f\t hy = %.5f\t tau = %.5f\n\n", N, hx, hy, tau);

		fprintf(density, "TITLE=\"density\"\n\nVARIABLES=\"x\",\"y\",\"Ro\"\n\n");

		fprintf(velocity, "TITLE=\"velocity\"\n\nVARIABLES=\"x\",\"y\",\"u\",\"v\"\n\n");

		fprintf(temperature, "TITLE=\"temperature\"\n\nVARIABLES=\"x\",\"y\",\"T\"\n\n");

		fprintf(pressure, "TITLE=\"pressure\"\n\nVARIABLES=\"x\",\"y\",\"P\"\n\n");
	}
	StartTimer();
	
	seq_init_arrays(M2, 12);

	//��������-������� ������� ��� t = 
	for (i = 0; i < qq; i++)
	{
		for (j = 0; j < M; j++)
		{
			a = i * M + j;

			if (i == 0)
			{
				Sigma_k1[a] = 1;
				T[a] = 1;
				e_k1[a] = sqrt(T[a] / (Gamma * (Gamma - 1) * Mah * Mah));

				u_k1[a] = 1;

				v_k1[a] = 0;

				e2[a] = e_k1[a];
				u2[a] = u_k1[a];
				v2[a] = v_k1[a];
			}

			if (i > 0 && i < qq)
			{
				Sigma_k1[a] = 1;
				T[a] = 1;
				e_k1[a] = sqrt(T[a] / (Gamma * (Gamma - 1) * Mah * Mah));

				u_k1[a] = 0;
				v_k1[a] = 0;

				e2[a] = e_k1[a];
				u2[a] = u_k1[a];
				v2[a] = v_k1[a];
			}
		}
	}

	for (i = qq; i < qq + w - 1; i++)
	{
		for (j = cntr + i - qq; j < M; j++)
		{
			a = i * M + j;


			Sigma_k1[a] = 1;
			T[a] = 1;
			e_k1[a] = sqrt(T[a] / (Gamma * (Gamma - 1) * Mah * Mah));
			u_k1[a] = 0;
			v_k1[a] = 0;

			e2[a] = e_k1[a];
			u2[a] = u_k1[a];
			v2[a] = v_k1[a];
		}
	}

	for (i = qq; i < qq + w - 1; i++)
	{
		for (j = cntr - i + qq; j > -1; j--)
		{
			a = i * M + j;


			Sigma_k1[a] = 1;
			T[a] = 1;
			e_k1[a] = sqrt(T[a] / (Gamma * (Gamma - 1) * Mah * Mah));
			u_k1[a] = 0;
			v_k1[a] = 0;

			e2[a] = e_k1[a];
			u2[a] = u_k1[a];
			v2[a] = v_k1[a];
		}
	}

	for (i = qq + w - 1; i < M1; i++)
	{
		for (j = 0; j < M; j++)
		{
			a = i * M + j;


			Sigma_k1[a] = 1;
			T[a] = 1;
			e_k1[a] = sqrt(T[a] / (Gamma * (Gamma - 1) * Mah * Mah));
			u_k1[a] = 0;
			v_k1[a] = 0;

			e2[a] = e_k1[a];
			u2[a] = u_k1[a];
			v2[a] = v_k1[a];
		}
	}

	/*	for(i = qq; i < qq+w-1; i++)
	{
	for(j = 0; j < i-qq; j++)
	{
	a = i*M + j;
	Sigma_k1[a] = 1;
	T[a] = 1;
	e_k1[a] = sqrt(T[a]/(Gamma*(Gamma-1)*Mah*Mah));
	u_k1[a] = 0;
	v_k1[a] = 0;

	}
	}
	*/
	/*-------------------------------------------------------------------*/
	/*-------��������� �������� �� ������� ���� ��� k = 0 ---------------*/
	/*			fprintf(out,"\t\t d = %i\t d*tau = %.5f\n\n",d,d*tau);
	fprintf(out,"q = %i\n\n",q);
	//fprintf(out,"|V|*tau = %.5f\n\n",sqrt(u_k1[M2-100]*u_k1[M2-100] + v_k1[M2-100]*v_k1[M2-100]));
	fprintf(out,"\t\t rho\t\t u\t\t v\t\t e\t\t P\t\t Mu\n\n",0);

	for(i = 0; i < M1; i++)
	{
	for(j = 0; j < M; j++)
	{

	a = i*M + j;
	fprintf(out, "i=%i j=%i\t %.10f\t %.10f\t %.10f\t %.10f %.10f\t %.10f\n",i,j,Sigma_k1[a]*Sigma_k1[a],u_k1[a],v_k1[a],e_k1[a]*e_k1[a],P(Sigma_k1[a],e_k1[a]),Mu(e_k1[a]));


	}
	}
	fprintf(out,"\n\n",0);
	/*-------------Time step begin ---------------*/
	/*-------------------------------------------------------------------*/
	d = 1;

	for (i = 0; i < M2; i++)
	{
		SigmaX_k[i] = 0;
		uX_k[i] = 0;
		vY_k[i] = 0;
		eR_k[i] = 0;
	}


	while (d <= m)
	{
		printf("Seq TS = %d\n", d);
		s_end = 0;

		for (i = 0; i < qq + 1; i++)
		{
			for (j = 0; j < M; j++)
			{
				a = i * M + j;
				Sigma_k[a] = Sigma_k1[a];
				e_k[a] = e_k1[a];
				u_k[a] = u_k1[a];
				v_k[a] = v_k1[a];
				Sigma_kk[a] = Sigma_k1[a];
				u_kk[a] = u_k1[a];
				v_kk[a] = v_k1[a];
				e_kk[a] = e_k1[a];
			}
		}

		for (i = qq; i < qq + w - 1; i++)
		{
			for (j = cntr + i - qq; j < M; j++)
			{
				a = i * M + j;
				Sigma_k[a] = Sigma_k1[a];
				e_k[a] = e_k1[a];
				u_k[a] = u_k1[a];
				v_k[a] = v_k1[a];
				Sigma_kk[a] = Sigma_k1[a];
				u_kk[a] = u_k1[a];
				v_kk[a] = v_k1[a];
				e_kk[a] = e_k1[a];
			}
		}

		for (i = qq; i < qq + w - 1; i++)
		{
			for (j = cntr - i + qq; j > -1; j--)
			{
				a = i * M + j;
				Sigma_k[a] = Sigma_k1[a];
				e_k[a] = e_k1[a];
				u_k[a] = u_k1[a];
				v_k[a] = v_k1[a];
				Sigma_kk[a] = Sigma_k1[a];
				u_kk[a] = u_k1[a];
				v_kk[a] = v_k1[a];
				e_kk[a] = e_k1[a];
			}
		}

		for (i = qq + w - 1; i < M1; i++)
		{
			for (j = 0; j < M; j++)
			{
				a = i * M + j;
				Sigma_k[a] = Sigma_k1[a];
				e_k[a] = e_k1[a];
				u_k[a] = u_k1[a];
				v_k[a] = v_k1[a];
				Sigma_kk[a] = Sigma_k1[a];
				u_kk[a] = u_k1[a];
				v_kk[a] = v_k1[a];
				e_kk[a] = e_k1[a];
			}
		}

		/*-----------------�������� �� ������������----------------------------*/

		for (s_itr = 1; s_itr < itr; s_itr++)
		{
			for (i = 1; i < qq + 1; i++)
			{
				for (j = 1; j < M - 1; j++)
				{
					a = i * M + j;

					SigmaX_k[a] = Sigma_kk[a] - trajectory(i, j, Sigma_kk, u_k[a], v_k[a]);
					uX_k[a] = u_kk[a] - trajectory(i, j, u_kk, u_k[a], v_k[a]);
					vY_k[a] = v_kk[a] - trajectory(i, j, v_kk, u_k[a], v_k[a]);
					eR_k[a] = e_kk[a] - trajectory(i, j, e_kk, u_k[a], v_k[a]);
				}
			}

			for (i = qq; i < qq + w - 1; i++)
			{
				for (j = cntr + i - qq; j < M - 1; j++)
				{
					a = i * M + j;

					SigmaX_k[a] = Sigma_kk[a] - trajectory(i, j, Sigma_kk, u_k[a], v_k[a]);
					uX_k[a] = u_kk[a] - trajectory(i, j, u_kk, u_k[a], v_k[a]);
					vY_k[a] = v_kk[a] - trajectory(i, j, v_kk, u_k[a], v_k[a]);
					eR_k[a] = e_kk[a] - trajectory(i, j, e_kk, u_k[a], v_k[a]);
				}
			}

			for (i = qq; i < qq + w - 1; i++)
			{
				for (j = cntr - i + qq; j > 0; j--)
				{
					a = i * M + j;

					SigmaX_k[a] = Sigma_kk[a] - trajectory(i, j, Sigma_kk, u_k[a], v_k[a]);
					uX_k[a] = u_kk[a] - trajectory(i, j, u_kk, u_k[a], v_k[a]);
					vY_k[a] = v_kk[a] - trajectory(i, j, v_kk, u_k[a], v_k[a]);
					eR_k[a] = e_kk[a] - trajectory(i, j, e_kk, u_k[a], v_k[a]);
				}
			}

			for (i = qq + w - 1; i < M1 - 1; i++)
			{
				for (j = 1; j < M - 1; j++)
				{
					a = i * M + j;

					SigmaX_k[a] = Sigma_kk[a] - trajectory(i, j, Sigma_kk, u_k[a], v_k[a]);
					uX_k[a] = u_kk[a] - trajectory(i, j, u_kk, u_k[a], v_k[a]);
					vY_k[a] = v_kk[a] - trajectory(i, j, v_kk, u_k[a], v_k[a]);
					eR_k[a] = e_kk[a] - trajectory(i, j, e_kk, u_k[a], v_k[a]);
				}
			}


			/*i = N1;
			for(j = 1; j < (M-1); j++)
			{
			a = i*M + j;

			if(u_k[a] < 0)
			{
			SigmaX_k[a] = Sigma_kk[a] - trajectory(i, j, Sigma_kk, 0, v_k[a]);
			uX_k[a] = u_kk[a] - trajectory(i, j, u_kk, 0, v_k[a]);
			vY_k[a] = v_kk[a] - trajectory(i, j, v_kk, 0, v_k[a]);
			eR_k[a] = e_kk[a] - trajectory(i, j, e_kk, 0, v_k[a]);
			}*/

			/*if(u_k[a] < 0)
			{
			SigmaX_k[a] = Sigma_kk[a];
			uX_k[a] = u_kk[a];
			vY_k[a] = v_kk[a];
			eR_k[a] = e_kk[a];
			}*/

			/*else
			{
			SigmaX_k[a] = Sigma_kk[a] - trajectory(i, j, Sigma_kk, u_k[a], v_k[a]);
			uX_k[a] = u_kk[a] - trajectory(i, j, u_kk, u_k[a], v_k[a]);
			vY_k[a] = v_kk[a] - trajectory(i, j, v_kk, u_k[a], v_k[a]);
			eR_k[a] = e_kk[a] - trajectory(i, j, e_kk, u_k[a], v_k[a]);
			}

			}*/

			/*j = 0;
			for(i = 1; i < M1-1; i++)
			{
			a = i*M + j;

			if(v_k[a] > 0)
			{
			SigmaX_k[a] = Sigma_kk[a] - trajectory(i, j, Sigma_kk, u_k[a], 0);
			uX_k[a] = u_kk[a] - trajectory(i, j, u_kk, u_k[a], 0);
			vY_k[a] = v_kk[a] - trajectory(i, j, v_kk, u_k[a], 0);
			eR_k[a] = e_kk[a] - trajectory(i, j, e_kk, u_k[a], 0);
			}
			else
			{
			SigmaX_k[a] = Sigma_kk[a] - trajectory(i, j, Sigma_kk, u_k[a], v_k[a]);
			uX_k[a] = u_kk[a] - trajectory(i, j, u_kk, u_k[a], v_k[a]);
			vY_k[a] = v_kk[a] - trajectory(i, j, v_kk, u_k[a], v_k[a]);
			eR_k[a] = e_kk[a] - trajectory(i, j, e_kk, u_k[a], v_k[a]);
			}
			}

			j = N;
			for(i = 1; i < (M1-1); i++)
			{
			a = i*M + j;

			if(v_k[a] < 0)
			{
			SigmaX_k[a] = Sigma_kk[a] - trajectory(i, j, Sigma_kk, u_k[a], 0);
			uX_k[a] = u_kk[a] - trajectory(i, j, u_kk, u_k[a], 0);
			vY_k[a] = v_kk[a] - trajectory(i, j, v_kk, u_k[a], 0);
			eR_k[a] = e_kk[a] - trajectory(i, j, e_kk, u_k[a], 0);
			}
			else
			{
			SigmaX_k[a] = Sigma_kk[a] - trajectory(i, j, Sigma_kk, u_k[a], v_k[a]);
			uX_k[a] = u_kk[a] - trajectory(i, j, u_kk, u_k[a], v_k[a]);
			vY_k[a] = v_kk[a] - trajectory(i, j, v_kk, u_k[a], v_k[a]);
			eR_k[a] = e_kk[a] - trajectory(i, j, e_kk, u_k[a], v_k[a]);
			}
			}


			i = N1;
			j = N;

			a = i*M + j;

			if((u_k[a] < 0) && (v_k[a] < 0))
			{
			SigmaX_k[a] = Sigma_kk[i*M+j];
			uX_k[a] = u_kk[i*M+j];
			vY_k[a] = v_kk[i*M + j];
			eR_k[a] = e_kk[i*M+j];
			}

			if((v_k[a] < 0) && (u_k[a] >= 0) )
			{
			SigmaX_k[a] = Sigma_kk[a] - trajectory(i, j, Sigma_kk, u_k[a], 0);
			uX_k[a] = u_kk[a] - trajectory(i, j, u_kk, u_k[a], 0);
			vY_k[a] = v_kk[a] - trajectory(i, j, v_kk, u_k[a], 0);
			eR_k[a] = e_kk[a] - trajectory(i, j, e_kk, u_k[a], 0);
			}
			if((u_k[a] < 0) && (v_k[a] >= 0))
			{
			SigmaX_k[a] = Sigma_kk[a] - trajectory(i, j, Sigma_kk, 0, v_k[a]);
			uX_k[a] = u_kk[a] - trajectory(i, j, u_kk, 0, v_k[a]);
			vY_k[a] = v_kk[a] - trajectory(i, j, v_kk, 0, v_k[a]);
			eR_k[a] = e_kk[a] - trajectory(i, j, e_kk, 0, v_k[a]);
			}

			if((u_k[a] >= 0) && (v_k[a] >= 0))
			{
			SigmaX_k[a] = Sigma_kk[a] - trajectory(i, j, Sigma_kk, u_k[a], v_k[a]);
			uX_k[a] = u_kk[a] - trajectory(i, j, u_kk, u_k[a], v_k[a]);
			vY_k[a] = v_kk[a] - trajectory(i, j, v_kk, u_k[a], v_k[a]);
			eR_k[a] = e_kk[a] - trajectory(i, j, e_kk, u_k[a], v_k[a]);
			}


			i = N1;
			j = 0;

			a = i*M + j;


			if((u_k[a] < 0) && (v_k[a] > 0))
			{
			SigmaX_k[a] = Sigma_kk[i*M+j];
			uX_k[a] = u_kk[i*M+j];
			vY_k[a] = v_kk[i*M + j];
			eR_k[a] = e_kk[i*M+j];
			}

			if((v_k[a] > 0) && (u_k[a] >= 0))
			{
			SigmaX_k[a] = Sigma_kk[a] - trajectory(i, j, Sigma_kk, u_k[a], 0);
			uX_k[a] = u_kk[a] - trajectory(i, j, u_kk, u_k[a], 0);
			vY_k[a] = v_kk[a] - trajectory(i, j, v_kk, u_k[a], 0);
			eR_k[a] = e_kk[a] - trajectory(i, j, e_kk, u_k[a], 0);
			}
			if((u_k[a] < 0) && (v_k[a] <= 0))
			{
			SigmaX_k[a] = Sigma_kk[a] - trajectory(i, j, Sigma_kk, 0, v_k[a]);
			uX_k[a] = u_kk[a] - trajectory(i, j, u_kk, 0, v_k[a]);
			vY_k[a] = v_kk[a] - trajectory(i, j, v_kk, 0, v_k[a]);
			eR_k[a] = e_kk[a] - trajectory(i, j, e_kk, 0, v_k[a]);
			}

			if((u_k[a] >= 0) && (v_k[a] <= 0))
			{
			SigmaX_k[a] = Sigma_kk[a] - trajectory(i, j, Sigma_kk, u_k[a], v_k[a]);
			uX_k[a] = u_kk[a] - trajectory(i, j, u_kk, u_k[a], v_k[a]);
			vY_k[a] = v_kk[a] - trajectory(i, j, v_kk, u_k[a], v_k[a]);
			eR_k[a] = e_kk[a] - trajectory(i, j, e_kk, u_k[a], v_k[a]);
			}*/

for (i = 0; i < M2; i++)
{
	e_k_mu[i] = Mu(e_k[i]);
}

			continuity(Sigma_k, Sigma_k1, u_k, v_k);
			motion(Sigma_k1, Sigma_k, u_k, v_k, u_k1, v_k1, u2, v2, e_k);
			energy(Sigma_k1, Sigma_k, u_k, v_k, u_k1, v_k1, e2, e_k, e_k1);

			if ((s_m == 1) && (s_e == 1))
			{
				s_end = s_itr;
				s_itr = itr;
			}

			i = 0;
			for (j = 0; j < M; j++)
			{
				a = i * M + j;
				Sigma_k[a] = Sigma_k1[a];
				e_k[a] = e_k1[a];
				u_k[a] = u_k1[a];
				v_k[a] = v_k1[a];
			}

			for (i = 1; i < qq + 1; i++)
			{
				for (j = 0; j < M; j++)
				{
					a = i * M + j;

					if (j == 0)
					{
						Sigma_k[a] = Sigma_k1[i * M + j + 1];
						Sigma_k1[a] = Sigma_k1[i * M + j + 1];
						e_k[a] = e_k1[i * M + j + 1];
						e_k1[a] = e_k1[i * M + j + 1];
						u_k[a] = u_k1[i * M + j + 1];
						u_k1[a] = u_k1[i * M + j + 1];
						v_k[a] = v_k1[i * M + j + 1];
						v_k1[a] = v_k1[i * M + j + 1];

						e2[a] = e_k1[i * M + j + 1];
						u2[a] = u_k1[i * M + j + 1];
						v2[a] = v_k1[i * M + j + 1];
					}

					if ((j > 0) && (j < N))
					{
						Sigma_k[a] = Sigma_k1[a];
						e_k[a] = e_k1[a];
						u_k[a] = u_k1[a];
						v_k[a] = v_k1[a];
					}

					if (j == N)
					{
						Sigma_k[a] = Sigma_k1[i * M + j - 1];
						Sigma_k1[a] = Sigma_k1[i * M + j - 1];
						e_k[a] = e_k1[i * M + j - 1];
						e_k1[a] = e_k1[i * M + j - 1];
						u_k[a] = u_k1[i * M + j - 1];
						u_k1[a] = u_k1[i * M + j - 1];
						v_k[a] = v_k1[i * M + j - 1];
						v_k1[a] = v_k1[i * M + j - 1];

						e2[a] = e_k1[i * M + j - 1];
						u2[a] = u_k1[i * M + j - 1];
						v2[a] = v_k1[i * M + j - 1];
					}
				}
			}

			for (i = qq; i < qq + w - 1; i++)
			{
				for (j = cntr + i - qq; j < M; j++)
				{
					a = i * M + j;

					if (j == N)
					{
						Sigma_k[a] = Sigma_k1[i * M + j - 1];
						Sigma_k1[a] = Sigma_k1[i * M + j - 1];
						e_k[a] = e_k1[i * M + j - 1];
						e_k1[a] = e_k1[i * M + j - 1];
						u_k[a] = u_k1[i * M + j - 1];
						u_k1[a] = u_k1[i * M + j - 1];
						v_k[a] = v_k1[i * M + j - 1];
						v_k1[a] = v_k1[i * M + j - 1];

						e2[a] = e_k1[i * M + j - 1];
						u2[a] = u_k1[i * M + j - 1];
						v2[a] = v_k1[i * M + j - 1];
					}

					else
					{
						Sigma_k[a] = Sigma_k1[a];
						e_k[a] = e_k1[a];
						u_k[a] = u_k1[a];
						v_k[a] = v_k1[a];
					}
				}
			}

			for (i = qq; i < qq + w - 1; i++)
			{
				for (j = cntr - i + qq; j > -1; j--)
				{
					a = i * M + j;

					if (j == 0)
					{
						Sigma_k[a] = Sigma_k1[i * M + j + 1];
						Sigma_k1[a] = Sigma_k1[i * M + j + 1];
						e_k[a] = e_k1[i * M + j + 1];
						e_k1[a] = e_k1[i * M + j + 1];
						u_k[a] = u_k1[i * M + j + 1];
						u_k1[a] = u_k1[i * M + j + 1];
						v_k[a] = v_k1[i * M + j + 1];
						v_k1[a] = v_k1[i * M + j + 1];

						e2[a] = e_k1[i * M + j + 1];
						u2[a] = u_k1[i * M + j + 1];
						v2[a] = v_k1[i * M + j + 1];
					}

					else
					{
						Sigma_k[a] = Sigma_k1[a];
						e_k[a] = e_k1[a];
						u_k[a] = u_k1[a];
						v_k[a] = v_k1[a];
					}
				}
			}

			for (i = qq + w - 1; i < M1 - 1; i++)
			{
				for (j = 0; j < M; j++)
				{
					a = i * M + j;

					if (j == 0)
					{
						Sigma_k[a] = Sigma_k1[i * M + j + 1];
						Sigma_k1[a] = Sigma_k1[i * M + j + 1];
						e_k[a] = e_k1[i * M + j + 1];
						e_k1[a] = e_k1[i * M + j + 1];
						u_k[a] = u_k1[i * M + j + 1];
						u_k1[a] = u_k1[i * M + j + 1];
						v_k[a] = v_k1[i * M + j + 1];
						v_k1[a] = v_k1[i * M + j + 1];

						e2[a] = e_k1[i * M + j + 1];
						u2[a] = u_k1[i * M + j + 1];
						v2[a] = v_k1[i * M + j + 1];
					}

					if ((j > 0) && (j < N))
					{
						Sigma_k[a] = Sigma_k1[a];
						e_k[a] = e_k1[a];
						u_k[a] = u_k1[a];
						v_k[a] = v_k1[a];
					}

					if (j == N)
					{
						Sigma_k[a] = Sigma_k1[i * M + j - 1];
						Sigma_k1[a] = Sigma_k1[i * M + j - 1];
						e_k[a] = e_k1[i * M + j - 1];
						e_k1[a] = e_k1[i * M + j - 1];
						u_k[a] = u_k1[i * M + j - 1];
						u_k1[a] = u_k1[i * M + j - 1];
						v_k[a] = v_k1[i * M + j - 1];
						v_k1[a] = v_k1[i * M + j - 1];

						e2[a] = e_k1[i * M + j - 1];
						u2[a] = u_k1[i * M + j - 1];
						v2[a] = v_k1[i * M + j - 1];
					}
				}
			}

			i = N1;
			for (j = 0; j < M; j++)
			{
				a = i * M + j;

				if (j == 0)
				{
					Sigma_k[a] = Sigma_k1[(i - 1) * M + j + 1];
					Sigma_k1[a] = Sigma_k1[(i - 1) * M + j + 1];
					e_k[a] = e_k1[(i - 1) * M + j + 1];
					e_k1[a] = e_k1[(i - 1) * M + j + 1];
					u_k[a] = u_k1[(i - 1) * M + j + 1];
					u_k1[a] = u_k1[(i - 1) * M + j + 1];
					v_k[a] = v_k1[(i - 1) * M + j + 1];
					v_k1[a] = v_k1[(i - 1) * M + j + 1];

					e2[a] = e_k1[(i - 1) * M + j + 1];
					u2[a] = u_k1[(i - 1) * M + j + 1];
					v2[a] = v_k1[(i - 1) * M + j + 1];
				}

				if ((j > 0) && (j < N))
				{
					Sigma_k[a] = Sigma_k1[(i - 1) * M + j];
					Sigma_k1[a] = Sigma_k1[(i - 1) * M + j];
					e_k[a] = e_k1[(i - 1) * M + j];
					e_k1[a] = e_k1[(i - 1) * M + j];
					u_k[a] = u_k1[(i - 1) * M + j];
					u_k1[a] = u_k1[(i - 1) * M + j];
					v_k[a] = v_k1[(i - 1) * M + j];
					v_k1[a] = v_k1[(i - 1) * M + j];

					e2[a] = e_k1[(i - 1) * M + j];
					u2[a] = u_k1[(i - 1) * M + j];
					v2[a] = v_k1[(i - 1) * M + j];
				}

				if (j == N)
				{
					Sigma_k[a] = Sigma_k1[(i - 1) * M + j - 1];
					Sigma_k1[a] = Sigma_k1[(i - 1) * M + j - 1];
					e_k[a] = e_k1[(i - 1) * M + j - 1];
					e_k1[a] = e_k1[(i - 1) * M + j - 1];
					u_k[a] = u_k1[(i - 1) * M + j - 1];
					u_k1[a] = u_k1[(i - 1) * M + j - 1];
					v_k[a] = v_k1[(i - 1) * M + j - 1];
					v_k1[a] = v_k1[(i - 1) * M + j - 1];

					e2[a] = e_k1[(i - 1) * M + j - 1];
					u2[a] = u_k1[(i - 1) * M + j - 1];
					v2[a] = v_k1[(i - 1) * M + j - 1];
				}
			}
		}

		/*---------------------------------------------------------------------------------------*/
		/*---------------����� ������ � ����--------------------*/


		if ((d / 1. == 1) || /*(d/2. == 1)||(d/3. == 1)||(d/4. == 1)||(d/5. == 1)||(d/10. == 1)||*/(d / 100. == 1)/*||(d/200. == 1)||(d/300. == 1)||(d/400. == 1)*/ || (d / 500. == 1)
			/*||(d/600. == 1)||(d/800. == 1)*/ || (d / 1000. == 1)/*||(d/1200. == 1)||(d/1500. == 1)||(d/1700. == 1)*/
			|| (d / 2000. == 1)/*||(d/2200. == 1)||(d/2500. == 1)||(d/2600. == 1)||(d/2700. == 1)||(d/2800. == 1)||(d/2900. == 1)*/
			|| (d / 3000. == 1)/*|| (d/3100. == 1)||(d/3200. == 1)|| (d/3300. == 1)|| (d/3400. == 1)*/
			|| (d / 3500. == 1)/*||(d/3600. == 1)||(d/3700. == 1)||(d/3800. == 1)||(d/3900. == 1)*/ || (d / 4000. == 1)/*||(d/4100. == 1)||(d/4200. == 1)||(d/4300. == 1)||(d/4400. == 1)*/ || (d / 4500. == 1)/*||(d/4600. == 1)||(d/4700. == 1)||(d/4800. == 1)||(d/4900. == 1)*/ || (d / 5000. == 1)/*|| (d/5100. == 1) || (d/5200. == 1)
																																																																									   || (d/5300. == 1)|| (d/5400. == 1)*/ || (d / 5500. == 1)/*|| (d/5600. == 1)|| (d/5700. == 1)|| (d/5800. == 1)|| (d/5900. == 1)*/ || (d / 6000. == 1)/*|| (d/6100. == 1)|| (d/6200. == 1)|| (d/6300. == 1)|| (d/6400. == 1)*/ || (d / 6500. == 1)/*|| (d/6700. == 1)*/ || (d / 7000. == 1) || (d / 7500. == 1) ||
			(d / 8000. == 1) || (d / 8500. == 1) || (d / 9000. == 1) || (d / 9500. == 1)
			|| (d / 10000. == 1) || (d / 11000. == 1) || (d / 12000. == 1)
			|| (d / 13000. == 1) || (d / 14000. == 1) || (d / 15000. == 1) || (d / 16000. == 1) || (d / 17000. == 1) || (d / 18000. == 1) || (d / 19000. == 1)
			|| (d / 20000. == 1)/*|| (d/21000. == 1)|| (d/22000. == 1)*/ || (d / 23000. == 1)/*|| (d/24000. == 1)*/ || (d / 25000. == 1)
			/*|| (d/26000. == 1)*/ || (d / 28000. == 1) || (d / 30000. == 1) || (d / 33000. == 1) || (d / 35000. == 1) || (d / 38000. == 1)
			|| (d / 40000. == 1) || (d / 43000. == 1) || (d / 45000. == 1) || (d / 48000. == 1) || (d / 50000. == 1) || (d / 53000. == 1) || (d / 55000. == 1)
			|| (d / 58000. == 1) || (d / 60000. == 1) || (d / 63000. == 1) || (d / 65000. == 1) || (d / 68000. == 1) || (d / 70000. == 1) || (d / 73000. == 1)
			|| (d / 75000. == 1) || (d / 78000. == 1) || (d / 80000. == 1) || (d / 83000. == 1) || (d / 85000. == 1) || (d / 88000. == 1) || (d / 90000. == 1)
			|| (d / 93000. == 1) || (d / 95000. == 1) || (d / 98000. == 1) || (d / 100000. == 1)
			|| (d / 103000. == 1) || (d / 105000. == 1) || (d / 108000. == 1) || (d / 110000. == 1) || (d / 113000. == 1) || (d / 115000. == 1) || (d / 118000. == 1)
			|| (d / 120000. == 1) || (d / 123000. == 1) || (d / 125000. == 1) || (d / 128000. == 1) || (d / 130000. == 1) || (d / 133000. == 1) || (d / 135000. == 1)
			|| (d / 138000. == 1) || (d / 140000. == 1) || (d / 143000. == 1) || (d / 145000. == 1) || (d / 148000. == 1) || (d / 150000. == 1)
			|| (d / 153000. == 1) || (d / 155000. == 1) || (d / 158000. == 1) || (d / 160000. == 1) || (d / 163000. == 1) || (d / 165000. == 1) || (d / 168000. == 1)
			|| (d / 170000. == 1) || (d / 173000. == 1) || (d / 175000. == 1) || (d / 178000. == 1) || (d / 180000. == 1) || (d / 183000. == 1) || (d / 185000. == 1)
			|| (d / 188000. == 1) || (d / 190000. == 1) || (d / 193000. == 1) || (d / 195000. == 1) || (d / 198000. == 1) || (d / 200000. == 1))
		{
			if (need_print)
			{
				/**** OUT PUT *****/
				fprintf(out, "\t\t d = %i\t d*tau = %.5f\n\n", d, d * tau);
				fprintf(out, "q = %i\t w = %i\n\n", q, w);

				fprintf(out_itr, "\t\t d = %i\t d*tau = %.5f\n\n", d, d * tau);
				if (s_end == 0)
				{
					fprintf(out_itr, "s_m = %i\t s_e = %i\t s_itr = %i\n\n", s_m, s_e, s_itr - 1);
					fprintf(out, "s_m = %i\t s_e = %i\t s_itr = %i\n\n", s_m, s_e, s_itr - 1);
				}
				else
				{
					fprintf(out_itr, "s_m = %i\t s_e = %i\t s_end = %i\n\n", s_m, s_e, s_end);
					fprintf(out, "s_m = %i\t s_e = %i\t s_end = %i\n\n", s_m, s_e, s_end);
				}

				fprintf(out, "\t\t rho\t\t u\t\t v\t\t e\t\t T\t\t P\t\t Mu\n\n");

				fprintf(density, "ZONE T=\"n = %i t = %.4f\",I=%i,J=%i,ZONETYPE=ORDERED,DATAPACKING=POINT\n\n", N, d * tau, M1, M);
				fprintf(velocity, "ZONE T=\"n = %i t = %.4f\",I=%i,J=%i,ZONETYPE=ORDERED,DATAPACKING=POINT\n\n", N, d * tau, M1, M);
				fprintf(temperature, "ZONE T=\"n = %i t = %.4f\",I=%i,J=%i,ZONETYPE=ORDERED,DATAPACKING=POINT\n\n", N, d * tau, M1, M);
				fprintf(pressure, "ZONE T=\"n = %i t = %.4f\",I=%i,J=%i,ZONETYPE=ORDERED,DATAPACKING=POINT\n\n", N, d * tau, M1, M);

				for (j = 0; j < M; j++)
				{
					for (i = 0; i < M1; i++)
					{
						a = i * M + j;

						fprintf(density, "%.3f\t %.4f\t %.10f\n", i * hx, j * hy, Sigma_k1[a] * Sigma_k1[a]);
						fprintf(density_new, "%.3f\t %.4f\t %.15e\n", i * hx, j * hy, Sigma_k1[a] * Sigma_k1[a]);
						fprintf(velocity, "%.3f\t %.4f\t%.10f\t %.10f\n", i * hx, j * hy, u_k1[a], v_k1[a]);
						fprintf(temperature, "%.3f\t %.4f\t %.10f\n", i * hx, j * hy, e_k1[a] * e_k1[a] * (Gamma * (Gamma - 1) * Mah * Mah));
						fprintf(pressure, "%.3f\t %.4f\t %.10f\n", i * hx, j * hy, Sigma_k1[a] * Sigma_k1[a] * e_k1[a] * e_k1[a] * (Gamma - 1));
					}
				}

				if (d == 1)
				{
					for (i = 0; i < M1; i++)
					{
						for (j = 0; j < M; j++)
						{
							a = i * M + j;
							fprintf(out, "i=%i j=%i\t %.10f\t %.10f\t %.10f\t %.10f\t %.10f\t %.10f\t %.10f\n", i, j, Sigma_k1[a] * Sigma_k1[a], u_k1[a], v_k1[a], e_k1[a] * e_k1[a], e_k1[a] * e_k1[a] * (Gamma * (Gamma - 1) * Mah * Mah), P(Sigma_k1[a], e_k1[a]), Mu(e_k1[a]));
						}
					}
				}

				fprintf(out, "\n\n");
				fprintf(density, "\n\n");
				fprintf(velocity, "\n\n");
				fprintf(temperature, "\n\n");
				fprintf(pressure, "\n\n");

				fflush(out);
				fflush(density);
				fflush(velocity);
				fflush(temperature);
				fflush(pressure);
				fflush(out_itr);
			}
		}
		/*---------------------------------------------------------------------------------------*/

		d += 1;
	}
	double time = GetTimer() / 1000;	
	if (need_print)
	{
		fprintf(out, "\n\n");
		fprintf(density, "\n\n");
		fprintf(velocity, "\n\n");
		fprintf(temperature, "\n\n");
		fprintf(pressure, "\n\n");
		fclose(out);
		fclose(density);
		fclose(velocity);
		fclose(temperature);
		fclose(pressure);
		fclose(out_itr);
	}
	return time;
}
