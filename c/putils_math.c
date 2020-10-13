





void powi(double x,int m)
{
    if (m<0) {
        m = -m;
        x = 1./x;
    }
    switch(m)
    {
    case 0: return 1.0;
    case 1: return x;
    case 2: return x*x;
    case 3: return x*x*x;
    case 4:
        y=x*x;
        return y*y;
    case 5:
        y=x*x;
        return y*y*x;
    case 6:
        y=x*x*x;
        return y*y;
    case 7:
        y=x*x*x;
        return y*y*x;
    case 8:
        y=x*x;
        y=y*y;
        return y*y;
    default:
        y=x*x;
        y=y*y;
        y=y*y;
        return y*powi(x,m-8);
    }
}

double dot_prod(const double *x,const double *y,size_t n)
{
    size_t i;
    double s=0.0;
    for (i=0;i<n;++i) s+=x[i]*y[i];
    return s;
}

double diff_nrm2(const double *x,const double *y,size_t n)
{
    size_t i;
    double t;
    double s=0.0;
    for (i=0;i<n;++i) {
        t=x[i]-y[i];
        s+=t*t;
    }
    return s;
}

void analyze(size_t ntp,size_t ntn,size_t nfp,size_t nfn)
{
    size_t nt = ntp + nfn;
    size_t nf = ntn + nfp;
    size_t np = ntp + nfp;
    size_t nn = ntn + nfn;
    size_t nc = ntp + ntn;
    size_t ne = nfp + nfn;
    size_t ntotal = nc+ne;
    
    double acc = ((double)nc)/((double)ntotal);
    double tpr = ((double)ntp)/((double)np);
    double tnr = ((double)ntn)/((double)nn);
    double ppv = ((double)ntp)/((double)nt);
    double npv = ((double)ntn)/((double)nf);
    
    fprintf(stderr,"

}