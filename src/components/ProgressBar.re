module Styles = {
  open Css;

  let container =
    style([
      display(`flex),
      position(`relative),
      width(`percent(100.)),
      height(`px(7)),
      borderRadius(`px(7)),
      backgroundColor(Colors.blueGray2),
    ]);

  let outer =
    style([
      display(`flex),
      width(`percent(100.)),
      height(`px(7)),
      borderRadius(`px(7)),
      overflow(`hidden),
    ]);

  let inner = p =>
    style([
      display(`flex),
      width(`percent(p)),
      height(`px(7)),
      background(Colors.bandBlue),
    ]);

  let withWidth = (w, p) =>
    style([
      display(`flex),
      flexDirection(`column),
      position(`absolute),
      width(`px(w)),
      top(`px(-22)),
      left(`calc((`sub, `percent(p), `px(w / 2)))),
      alignItems(`center),
    ]);

  let arrowDown =
    style([
      width(`zero),
      height(`zero),
      borderLeft(`px(6), `solid, Colors.transparent),
      borderRight(`px(6), `solid, Colors.transparent),
      borderTop(`px(6), `solid, Colors.bandBlue),
    ]);

  let lowerText =
    style([
      position(`absolute),
      display(`flex),
      width(`percent(100.)),
      justifyContent(`center),
      top(`px(10)),
    ]);
};

[@react.component]
let make = (~reportedValidators, ~minimumValidators, ~requestValidators) => {
  let minimumPercentage =
    (minimumValidators * 100 |> float_of_int) /. (requestValidators |> float_of_int);
  let progressPercentage =
    (reportedValidators * 100 |> float_of_int) /. (requestValidators |> float_of_int);
  <div className=Styles.container>
    <div className=Styles.outer> <div className={Styles.inner(progressPercentage)} /> </div>
    <div className={Styles.withWidth(120, minimumPercentage)}>
      <Text value={"minimum: " ++ (minimumValidators |> Format.iPretty)} color=Colors.bandBlue />
      <div className=Styles.arrowDown />
    </div>
    {reportedValidators < minimumValidators
       ? <div className=Styles.lowerText>
           <Text
             value={
               "Pending "
               ++ (minimumValidators - reportedValidators |> Format.iPretty)
               ++ " Validators"
             }
             color=Colors.gray7
             size=Text.Sm
           />
         </div>
       : React.null}
  </div>;
};
