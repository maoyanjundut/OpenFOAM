function [mi,MAX,c,C,d,D]=FindHalfPeak(pks,t,locs)
 Max=0;
for i=1:1:length(pks)
   
if pks(i)>Max
    Max=pks(i);
    a=i;
else continue
end
end
mi=a
MAX=Max
C=Max;
c=0;
for j=1:1:length(pks)
    if pks(j)>(1/2*Max-0.02*Max)&&pks(j)<(1/2*Max+0.02*Max)%matlab中应用和C一样的逻辑表达方式
      C=pks(j);
      c=j;
      break;
     else continue
    end
end
c
C
d=0;
D=Max;
for k=1:1:length(pks)
    if pks(k)>(1/5*Max-0.02*Max)&&pks(k)<(1/5*Max+0.02*Max)
      D=pks(k);
      d=k;
      break;
      else continue
    end
end
d
D
if c==0
    disp('the erro band is too narrow')
else THalf=t(locs(c))-t(locs(a))
end
if d==0
    disp('the erro band is too narrow')
else TFIVE=t(locs(d))-t(locs(a))
end
end
