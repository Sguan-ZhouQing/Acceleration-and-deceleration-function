# Acceleration-and-deceleration-function
T-shaped and S-shaped curve acceleration

void SVPWM(float phi, float d, float q, float *d_u, float *d_v, float *d_w) {
    d = Sg_Limit(d,1,-1);   // 限幅函数
    q = Sg_Limit(q,1,-1);
    const int v[6][3] = {{1, 0, 0}, {1, 1, 0}, {0, 1, 0}, {0, 1, 1}, {0, 0, 1}, {1, 0, 1}};
    const int K_to_sector[] = {4, 6, 5, 5, 3, 1, 2, 2};
    float sin_phi,cos_phi;
    fast_sin_cos(phi,&sin_phi,&cos_phi);
    float alpha,beta;
    ipark(&alpha, &beta, d, q, sin_phi, cos_phi);

    bool A = beta > 0;
    bool B = fabsf(beta) > SQRT3 * fabsf(alpha);
    bool C = alpha > 0;
    int K = 4 * A + 2 * B + C;
    int sector = K_to_sector[K];

    float angle_data0 = sector * rad60;
    float angle_data1 = angle_data0 - rad60;
    float sin_m,cos_m,sin_n,cos_n;
    fast_sin_cos(angle_data0,&sin_m,&cos_m);
    fast_sin_cos(angle_data1,&sin_n,&cos_n);

    float t_m = sin_m * alpha - cos_m * beta;
    float t_n = beta * cos_n - alpha * sin_n;
    float t_0 = 1 - t_m - t_n;
    *d_u = t_m * v[sector - 1][0] + t_n * v[sector % 6][0] + t_0 / 2;
    *d_v = t_m * v[sector - 1][1] + t_n * v[sector % 6][1] + t_0 / 2;
    *d_w = t_m * v[sector - 1][2] + t_n * v[sector % 6][2] + t_0 / 2;
}
