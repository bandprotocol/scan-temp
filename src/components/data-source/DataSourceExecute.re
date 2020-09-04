module Styles = {
  open Css;

  let container = style([padding2(~h=`px(20), ~v=`px(20))]);

  let paramsContainer = style([display(`flex), flexDirection(`column)]);

  let listContainer = style([marginBottom(`px(25))]);

  let input =
    style([
      width(`percent(100.)),
      background(white),
      paddingLeft(`px(20)),
      fontSize(`px(12)),
      fontWeight(`num(500)),
      outline(`px(1), `none, white),
      height(`px(40)),
      borderRadius(`px(4)),
      boxShadow(
        Shadow.box(~inset=true, ~x=`zero, ~y=`zero, ~blur=`px(4), Css.rgba(0, 0, 0, 0.1)),
      ),
    ]);

  let buttonContainer = style([display(`flex), flexDirection(`row), alignItems(`center)]);

  let button = isLoading =>
    style([
      width(`px(110)),
      backgroundColor(isLoading ? Colors.blueGray3 : Colors.green2),
      borderRadius(`px(6)),
      fontSize(`px(12)),
      fontWeight(`num(600)),
      color(isLoading ? Colors.blueGray7 : Colors.green7),
      cursor(isLoading ? `auto : `pointer),
      padding2(~v=Css.px(10), ~h=Css.px(10)),
      whiteSpace(`nowrap),
      outline(`zero, `none, white),
      boxShadow(
        isLoading
          ? `none : Shadow.box(~x=`zero, ~y=`px(2), ~blur=`px(4), Css.rgba(0, 0, 0, 0.1)),
      ),
      border(`zero, `solid, Colors.white),
    ]);

  let hFlex = style([display(`flex), flexDirection(`row)]);

  let withWH = (w, h) =>
    style([
      width(w),
      height(h),
      display(`flex),
      justifyContent(`center),
      alignItems(`center),
    ]);

  let resultWrapper = (w, h, overflowChioce) =>
    style([
      width(w),
      height(h),
      display(`flex),
      flexDirection(`column),
      justifyContent(`center),
      backgroundColor(Colors.white),
      borderRadius(`px(4)),
      overflow(overflowChioce),
    ]);
};

let parameterInput = (name, index, setCalldataList) => {
  <div className=Styles.listContainer key=name>
    <Text value=name size=Text.Md color=Colors.gray6 />
    <VSpacing size=Spacing.xs />
    <input
      className=Styles.input
      type_="text"
      onChange={event => {
        let newVal = ReactEvent.Form.target(event)##value;
        setCalldataList(prev => {
          prev->Belt_List.mapWithIndex((i, value) => {index == i ? newVal : value})
        });
      }}
    />
  </div>;
};

type result_data_t = {
  returncode: int,
  stdout: string,
  stderr: string,
};

type result_t =
  | Nothing
  | Loading
  | Error(string)
  | Success(result_data_t);

let loadingRender = (wDiv, wImg, h) => {
  <div className={Styles.withWH(wDiv, h)}>
    <img src=Images.loadingCircles className={Styles.withWH(wImg, h)} />
  </div>;
};

let resultRender = result => {
  switch (result) {
  | Nothing => React.null
  | Loading =>
    <>
      <VSpacing size=Spacing.xl />
      {loadingRender(`percent(100.), `px(104), `px(30))}
      <VSpacing size=Spacing.lg />
    </>
  | Error(err) =>
    <>
      <VSpacing size=Spacing.lg />
      <div className={Styles.resultWrapper(`percent(100.), `px(90), `scroll)}>
        <Text value=err />
      </div>
    </>
  | Success({returncode, stdout, stderr}) =>
    <>
      <VSpacing size=Spacing.lg />
      <div className={Styles.resultWrapper(`percent(100.), `px(95), `auto)}>
        <div className=Styles.hFlex>
          <HSpacing size=Spacing.lg />
          <div className={Styles.resultWrapper(`px(120), `px(12), `auto)}>
            <Text value="Exit Status" color=Colors.gray6 weight=Text.Semibold />
          </div>
          <Text value={returncode |> string_of_int} />
        </div>
        <VSpacing size=Spacing.md />
        <div className=Styles.hFlex>
          <HSpacing size=Spacing.lg />
          <div className={Styles.resultWrapper(`px(120), `px(12), `auto)}>
            <Text value="Output" color=Colors.gray6 weight=Text.Semibold />
          </div>
          <Text value=stdout code=true weight=Text.Semibold />
        </div>
        <VSpacing size=Spacing.md />
        <div className=Styles.hFlex>
          <HSpacing size=Spacing.lg />
          <div className={Styles.resultWrapper(`px(120), `px(12), `auto)}>
            <Text value="Error" color=Colors.gray6 weight=Text.Semibold />
          </div>
          <Text value=stderr code=true weight=Text.Semibold />
        </div>
      </div>
    </>
  };
};

[@react.component]
let make = (~executable: JsBuffer.t) => {
  let params =
    ExecutableParser.parseExecutableScript(executable)->Belt_Option.getWithDefault([]);
  let numParams = params->Belt_List.length;

  let (callDataList, setCalldataList) = React.useState(_ => Belt_List.make(numParams, ""));

  let (result, setResult) = React.useState(_ => Nothing);

  <div className=Styles.container>
    <div className=Styles.hFlex>
      <Text
        value={
          "Test data source execution"
          ++ (numParams == 0 ? "" : " with" ++ (numParams == 1 ? " a " : " ") ++ "following")
        }
        color=Colors.gray7
      />
      <HSpacing size=Spacing.sm />
      {numParams == 0
         ? React.null
         : <Text
             value={numParams > 1 ? "parameters" : "parameter"}
             color=Colors.gray7
             weight=Text.Bold
           />}
    </div>
    <VSpacing size=Spacing.lg />
    {numParams > 0
       ? <div className=Styles.paramsContainer>
           {params
            ->Belt_List.mapWithIndex((i, param) => parameterInput(param, i, setCalldataList))
            ->Belt_List.toArray
            ->React.array}
         </div>
       : React.null}
    <VSpacing size=Spacing.md />
    <div className=Styles.buttonContainer>
      <button
        className={Styles.button(result == Loading)}
        onClick={_ =>
          if (result != Loading) {
            setResult(_ => Loading);
            let _ =
              AxiosRequest.execute(
                AxiosRequest.t(
                  ~executable=executable->JsBuffer.toBase64,
                  ~calldata={
                    callDataList
                    ->Belt_List.reduce("", (acc, calldata) => acc ++ " " ++ calldata)
                    ->String.trim;
                  },
                  ~timeout=5000,
                ),
              )
              |> Js.Promise.then_(res => {
                   setResult(_ =>
                     Success({
                       returncode: res##data##returncode,
                       stdout: res##data##stdout,
                       stderr: res##data##stderr,
                     })
                   );
                   Js.Promise.resolve();
                 })
              |> Js.Promise.catch(err => {
                   let errorValue =
                     Js.Json.stringifyAny(err)->Belt_Option.getWithDefault("Unknown");
                   setResult(_ => Error(errorValue));
                   Js.Promise.resolve();
                 });
            ();
          }
        }>
        {(result == Loading ? "Executing ... " : "Test Execution") |> React.string}
      </button>
    </div>
    {resultRender(result)}
  </div>;
};
