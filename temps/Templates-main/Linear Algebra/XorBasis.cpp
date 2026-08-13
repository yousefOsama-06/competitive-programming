struct Basis{
    vector<ll>basis;
    int LOG,sz;
    Basis(int log = 61){
        LOG = log;
        sz = 0;
        basis.resize(LOG);
    }
    bool insert(ll x){
        for (int i = LOG - 1; i >= 0; --i) {
            if(x >> i & 1 ^ 1)continue;
            if(basis[i]){
                x ^= basis[i];
            }
            else{
                basis[i] = x;
                sz++;
                return true;
            }
        }
        return false;
    }
    ll getMax(ll x = 0){
        ll ret = x;
        for (int i = LOG - 1; i >= 0; --i) {
            ret = max(ret,ret ^ basis[i]);
        }
        return ret;
    }
};
