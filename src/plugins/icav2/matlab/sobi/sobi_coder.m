% sobi() - Second Order Blind Identification (SOBI) by joint diagonalization of
%          correlation  matrices. THIS CODE ASSUMES TEMPORALLY CORRELATED SIGNALS,
%          and uses correlations across times in performing the signal separation. 
%          Thus, estimated time delayed covariance matrices must be nonsingular 
%          for at least some time delays. 
% Usage:  
%         >> winv = sobi(data);
%         >> [winv,act] = sobi(data,n,p);
% Inputs: 
%   data - data matrix of size [m,N] ELSE of size [m,N,t] where
%                m is the number of sensors,
%                N is the  number of samples, 
%                t is the  number of trials (avoid epoch boundaries)
%         n - number of sources {Default: n=m}
%         p - number of correlation matrices to be diagonalized 
%             {Default: min(100, N/3)} Note that for non-ideal data, 
%             the authors strongly recommend using at least 100 time delays.
%
% Outputs:
%   winv - Matrix of size [m,n], an estimate of the *mixing* matrix. Its
%          columns are the component scalp maps. NOTE: This is the inverse
%          of the usual ICA unmixing weight matrix. Sphering (pre-whitening),
%          used in the algorithm, is incorporated into winv. i.e.,
%
%             >> icaweights = pinv(winv); icasphere = eye(m);
% 
%   act  - matrix of dimension [n,N] an estimate of the source activities 
%
%             >> data            = winv            * act; 
%                [size m,N]        [size m,n]        [size n,N]
%             >> act = pinv(winv) * data;
%
% Authors:  A. Belouchrani and A. Cichocki (references: See function body)
% Note:     Adapted by Arnaud Delorme and Scott Makeig to process data epochs by
%           computing covariances while respecting epoch boundaries.
 
% REFERENCES:
% A. Belouchrani, K. Abed-Meraim, J.-F. Cardoso, and E. Moulines, ``Second-order
%  blind separation of temporally correlated sources,'' in Proc. Int. Conf. on
%  Digital Sig. Proc., (Cyprus), pp. 346--351, 1993.
%
%  A. Belouchrani and K. Abed-Meraim, ``Separation aveugle au second ordre de
%  sources correlees,'' in  Proc. Gretsi, (Juan-les-pins), 
%  pp. 309--312, 1993.
%
%  A. Belouchrani, and A. Cichocki, 
%  Robust whitening procedure in blind source separation context, 
%  Electronics Letters, Vol. 36, No. 24, 2000, pp. 2050-2053.
%  
%  A. Cichocki and S. Amari, 
%  Adaptive Blind Signal and Image Processing, Wiley,  2003.

function [mixing, unmixing]=sobi_coder(data, ncomps)

% Authors note: For non-ideal data, use at least p=100 the time-delayed covariance matrices.
DEFAULT_LAGS = 100;

[m,N]=size(data);

if m > ncomps
    data = pca(data, ncomps);
    [m,N] = size(data);
end

%n=m; % Source detection (hum...)
 p=min(DEFAULT_LAGS,ceil(N/3)); % Number of time delayed correlation matrices to be diagonalized 


%
% Make the data zero mean
%
data(:,:)=data(:,:)-kron(mean(data(:,:)')',ones(1,N)); 

%
% Pre-whiten the data based directly on SVD
%
[~,S,VV]=svd(data(:,:)',0);
Q= pinv(S)*VV';
data(:,:)=Q*data(:,:);

% Alternate whitening code
% Rx=(X*X')/T;
% if m<n, % assumes white noise
%   [U,D]=eig(Rx); 
%   [puiss,k]=sort(diag(D));
%   ibl= sqrt(puiss(n-m+1:n)-mean(puiss(1:n-m)));
%    bl = ones(m,1) ./ ibl ;
%   BL=diag(bl)*U(1:n,k(n-m+1:n))';
%   IBL=U(1:n,k(n-m+1:n))*diag(ibl);
% else    % assumes no noise
%    IBL=sqrtm(Rx);
%    Q=inv(IBL);
% end;
% X=Q*X;

%
% Estimate the correlation matrices
%
 k=1;
 pm=p*m; % for convenience
 M = [];
 for u=1:m:pm
   k=k+1; 
   Rxp=data(:,k:N)*data(:,1:N-k+1)'/(N-k+1);
   M(:,u:u+m-1)=norm(Rxp,'fro')*Rxp;  % Frobenius norm =
 end                                % sqrt(sum(diag(Rxp'*Rxp)))

%
% Perform joint diagonalization
%
epsil=1/sqrt(N)/100; 

V=eye(m);
step_n=0;
encore = true;
while encore 
 encore = false;
 for p=1:m-1
  for q=p+1:m
   % Perform Givens rotation
   g=[   M(p,p:m:pm)-M(q,q:m:pm)  ;
         M(p,q:m:pm)+M(q,p:m:pm)  ;
      1i*(M(q,p:m:pm)-M(p,q:m:pm)) ];
	  [vcp,D] = eig(real(g*g')); 
          [~,K]=sort(diag(D));
   angles=vcp(:,K(3));
   angles=sign(angles(1))*angles;
   c=sqrt(0.5+angles(1)/2);
   sr=0.5*(angles(2)-1j*angles(3))/c; 
   sc=conj(sr);ù
   oui = abs(sr)> epsil ;
   encore= encore | oui ;
   if oui  % Update the M and V matrices 
    colp=M(:,p:m:pm);
    colq=M(:,q:m:pm);
    M(:,p:m:pm)=real(c*colp+sr*colq);
    M(:,q:m:pm)=real(c*colq-sc*colp);
    rowp=M(p,:);
    rowq=M(q,:);
    M(p,:)=real(c*rowp+sc*rowq);
    M(q,:)=real(c*rowq-sr*rowp);
    temp=V(:,p);
    V(:,p)=real(c*V(:,p)+sr*V(:,q));
    V(:,q)=real(c*V(:,q)-sc*temp);
   end%% if
  end%% q loop
 end%% p loop
 step_n=step_n+1;
end%% while

%
% Estimate the mixing matrix 
%
mixing = pinv(Q)*V; 
unmixing = pinv(mixing);
end

