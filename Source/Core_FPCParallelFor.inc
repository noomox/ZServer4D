type
  TFPCParallelThData32 = record
    b, e: Integer;
    Completed: ^Integer;
    OnFor: TFPCParallelForProcedure32;
    Critical: TCritical;
  end;

  PFPCParallelThData32 = ^TFPCParallelThData32;

procedure FPCParallelTh32(ThSender: TComputeThread);
var
  p: PFPCParallelThData32;
  Pass: Integer;
begin
  p := ThSender.UserData;
  for Pass := p^.b to p^.e do
      p^.OnFor(Pass);

  p^.Critical.Acquire;
  AtomInc(p^.Completed^, p^.e - p^.b + 1);
  p^.Critical.Release;
  dispose(p);
end;

procedure FPCParallelFor(OnFor: TFPCParallelForProcedure32; b, e: Integer);
var
  Total, Depth, Completed, StepTotal, stepW, Pass, w: Integer;
  p: PFPCParallelThData32;
  i_: Integer;
  Critical: TCritical;
begin
  if b > e then
      Swap(b, e);
  if ParallelOverflow.Busy() then
    begin
      for i_ := b to e do
          OnFor(i_);
      exit;
    end;
  ParallelOverflow.Acquire;
  try
    Depth := ParallelGranularity;
    Total := e - b + 1;
    Critical := TCritical.Create;

    Completed := 0;

    if (Total < Depth) then
      begin
        for Pass := b to e do
          begin
            new(p);
            p^.b := Pass;
            p^.e := Pass;
            p^.Completed := @Completed;
            p^.OnFor := OnFor;
            p^.Critical := Critical;
            TComputeThread.RunC(p, nil, @FPCParallelTh32);
          end;
      end
    else
      begin
        stepW := Total div Depth;
        StepTotal := Total div stepW;
        if Total mod stepW > 0 then
            inc(StepTotal);

        for Pass := 0 to StepTotal - 1 do
          begin
            w := stepW * Pass;
            new(p);
            if w + stepW <= Total then
              begin
                p^.b := w + b;
                p^.e := w + stepW + b - 1;
              end
            else
              begin
                p^.b := w + b;
                p^.e := Total + b - 1;
              end;
            p^.Completed := @Completed;
            p^.OnFor := OnFor;
            p^.Critical := Critical;
            TComputeThread.RunC(p, nil, @FPCParallelTh32);
          end;
      end;

    repeat
      TThread.Sleep(1);
      Critical.Acquire;
      w := Completed;
      Critical.Release;
    until w >= Total;

    Critical.Free;
  finally
      ParallelOverflow.Release;
  end;
end;

type
  TFPCParallelThData64 = record
    b, e: Int64;
    Completed: ^Int64;
    OnFor: TFPCParallelForProcedure64;
    Critical: TCritical;
  end;

  PFPCParallelThData64 = ^TFPCParallelThData64;

procedure FPCParallelTh64(ThSender: TComputeThread);
var
  p: PFPCParallelThData64;
  Pass: Int64;
begin
  p := ThSender.UserData;
  for Pass := p^.b to p^.e do
      p^.OnFor(Pass);

  p^.Critical.Acquire;
  AtomInc(p^.Completed^, p^.e - p^.b + 1);
  p^.Critical.Release;
  dispose(p);
end;

procedure FPCParallelFor(OnFor: TFPCParallelForProcedure64; b, e: Int64);
var
  Total, Depth, Completed, StepTotal, stepW, Pass, w: Int64;
  p: PFPCParallelThData64;
  i_: Int64;
  Critical: TCritical;
begin
  if b > e then
      Swap(b, e);
  if ParallelOverflow.Busy() then
    begin
      for i_ := b to e do
          OnFor(i_);
      exit;
    end;
  ParallelOverflow.Acquire;
  try
    Depth := ParallelGranularity;
    Total := e - b + 1;
    Critical := TCritical.Create;

    Completed := 0;

    if (Total < Depth) then
      begin
        for Pass := b to e do
          begin
            new(p);
            p^.b := Pass;
            p^.e := Pass;
            p^.Completed := @Completed;
            p^.OnFor := OnFor;
            p^.Critical := Critical;
            TComputeThread.RunC(p, nil, @FPCParallelTh64);
          end;
      end
    else
      begin
        stepW := Total div Depth;
        StepTotal := Total div stepW;
        if Total mod stepW > 0 then
            inc(StepTotal);

        for Pass := 0 to StepTotal - 1 do
          begin
            w := stepW * Pass;
            new(p);
            if w + stepW <= Total then
              begin
                p^.b := w + b;
                p^.e := w + stepW + b - 1;
              end
            else
              begin
                p^.b := w + b;
                p^.e := Total + b - 1;
              end;
            p^.Completed := @Completed;
            p^.OnFor := OnFor;
            p^.Critical := Critical;
            TComputeThread.RunC(p, nil, @FPCParallelTh64);
          end;
      end;

    repeat
      TThread.Sleep(1);
      Critical.Acquire;
      w := Completed;
      Critical.Release;
    until w >= Total;

    Critical.Free;
  finally
      ParallelOverflow.Release;
  end;
end;