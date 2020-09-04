module Styles = {
  open Css;

  let tableLowerContainer = style([padding(`px(8))]);
  let tableWrapper = style([padding2(~v=`px(20), ~h=`px(15))]);
  let codeImage = style([width(`px(20)), marginRight(`px(10))]);
  let vFlex = style([display(`flex), flexDirection(`row), alignItems(`center)]);

  let scriptContainer =
    style([
      fontSize(`px(12)),
      lineHeight(`px(20)),
      fontFamilies([
        `custom("IBM Plex Mono"),
        `custom("cousine"),
        `custom("sfmono-regular"),
        `custom("Consolas"),
        `custom("Menlo"),
        `custom("liberation mono"),
        `custom("ubuntu mono"),
        `custom("Courier"),
        `monospace,
      ]),
    ]);

  let padding = style([padding(`px(20))]);

  let selectWrapper =
    style([
      display(`flex),
      flexDirection(`row),
      padding2(~v=`px(3), ~h=`px(8)),
      position(`static),
      width(`px(169)),
      height(`px(30)),
      left(`zero),
      top(`px(32)),
      background(rgba(255, 255, 255, 1.)),
      borderRadius(`px(100)),
      boxShadow(Shadow.box(~x=`zero, ~y=`px(4), ~blur=`px(4), rgba(0, 0, 0, 0.1))),
      float(`left),
    ]);

  let selectContent =
    style([
      background(rgba(255, 255, 255, 1.)),
      border(`px(0), `solid, hex("FFFFFF")),
      width(`px(169)),
      float(`right),
    ]);

  let iconWrapper = style([display(`flex), alignItems(`center), justifyContent(`center)]);

  let iconBody = style([width(`px(20)), height(`px(20))]);

  let languageOption = style([display(`flex), flexDirection(`row), alignContent(`center)]);

  let languageText = style([alignItems(`center), display(`flex)]);
};

let renderCode = content => {
  <div className=Styles.scriptContainer>
    <ReactHighlight className=Styles.padding> {content |> React.string} </ReactHighlight>
  </div>;
};

type target_platform_t =
  | Ethereum
  | CosmosIBC;
/*   | Kadena; */

type language_t =
  | Solidity
  /*   | Vyper */
  | Go;
/*   | PACT; */

exception WrongLanugageChoice(string);
exception WrongPlatformChoice(string);

let toLanguageVariant =
  fun
  | "Solidity" => Solidity
  /* | "Vyper" => Vyper */
  | "Go" => Go
  /*   | "PACT" => PACT */
  | _ => raise(WrongLanugageChoice("Chosen language does not exist"));

let toPlatformVariant =
  fun
  | "Ethereum" => Ethereum
  | "Cosmos IBC" => CosmosIBC
  /*   | "Kadena" => Kadena */
  | _ => raise(WrongPlatformChoice("Chosen platform does not exist"));

let toLanguageString =
  fun
  | Solidity => "Solidity"
  /*   | Vyper => "Vyper" */
  | Go => "Go";
/*  | PACT => "PACT"; */

let toPlatformString =
  fun
  | Ethereum => "Ethereum"
  | CosmosIBC => "Cosmos IBC";
/*   | Kadena => "Kadena"; */

let getLanguagesByPlatform =
  fun
  //TODO: Add back Vyper
  | Ethereum => [|Solidity|]
  | CosmosIBC => [|Go|];
/*   | Kadena => [|PACT|]; */

module TargetPlatformIcon = {
  [@react.component]
  let make = (~icon) => {
    <div className=Styles.iconWrapper>
      <img
        className=Styles.iconBody
        src={
          switch (icon) {
          | Ethereum => Images.ethereumIcon
          | CosmosIBC => Images.cosmosIBCIcon
          /*           | Kadena => Images.kadenaIcon */
          }
        }
      />
    </div>;
  };
};

module LanguageIcon = {
  [@react.component]
  let make = (~icon) => {
    <div className=Styles.iconWrapper>
      <img
        className=Styles.iconBody
        src={
          switch (icon) {
          | Solidity => Images.solidityIcon
          /*           | Vyper => Images.vyperIcon */
          | Go => Images.golangIcon
          /*           | PACT => Images.pactIcon */
          }
        }
      />
    </div>;
  };
};

let getFileNameFromLanguage = (~language, ~dataType) => {
  let dataTypeString = dataType |> Obi.dataTypeToString;
  switch (language) {
  | Solidity => "Decoders.sol"
  | Go => {j|$(dataTypeString)Decoder.go|j}
  };
};

let getCodeFromSchema = (~schema, ~language, ~dataType) => {
  switch (language) {
  | Solidity => Obi.generateDecoderSolidity(schema)
  | Go => Obi.generateDecoderGo("main", schema, dataType)
  };
};

module GenerateDecodeCode = {
  [@react.component]
  let make = (~language, ~schema, ~dataType) => {
    let codeOpt = getCodeFromSchema(~schema, ~language, ~dataType);
    let code =
      switch (codeOpt) {
      | Some(code) => code
      | _ => "Code is not available."
      };
    <div className=Styles.tableLowerContainer>
      <div className=Styles.vFlex>
        <img src=Images.code className=Styles.codeImage />
        <Text
          value={getFileNameFromLanguage(~language, ~dataType)}
          size=Text.Lg
          color=Colors.gray7
        />
        <HSpacing size=Spacing.md />
        <CopyRender width=15 message=code />
      </div>
      <VSpacing size=Spacing.lg />
      code->renderCode
    </div>;
  };
};

[@react.component]
let make = (~schema) => {
  let (targetPlatform, setTargetPlatform) = React.useState(_ => Ethereum);
  let (language, setLanguage) = React.useState(_ => Solidity);
  <div className=Styles.tableWrapper>
    <VSpacing size={`px(10)} />
    <Row>
      <HSpacing size={`px(15)} />
      <Col> <div> <Text value="Target Platform" /> </div> </Col>
      <HSpacing size={`px(370)} />
    </Row>
    <Row>
      <Col size=1.>
        <VSpacing size={`px(5)} />
        <div className=Styles.selectWrapper>
          <TargetPlatformIcon icon=targetPlatform />
          <select
            className=Styles.selectContent
            onChange={event => {
              let newPlatform = ReactEvent.Form.target(event)##value |> toPlatformVariant;
              setTargetPlatform(_ => newPlatform);
              let newLanguage = newPlatform |> getLanguagesByPlatform |> Belt_Array.getExn(_, 0);
              setLanguage(_ => newLanguage);
            }}>
            // TODO: Add back Kadena and Cosmos

              {[|Ethereum|]
               ->Belt_Array.map(symbol =>
                   <option value={symbol |> toPlatformString}>
                     {symbol |> toPlatformString |> React.string}
                   </option>
                 )
               |> React.array}
            </select>
        </div>
      </Col>
      <Col size=1.>
        <div className=Styles.languageOption>
          <div className=Styles.languageText> <Text value="Language" /> </div>
          <HSpacing size={`px(15)} />
          <div className=Styles.selectWrapper>
            <LanguageIcon icon=language />
            <select
              className=Styles.selectContent
              onChange={event => {
                let newLanguage = ReactEvent.Form.target(event)##value |> toLanguageVariant;
                setLanguage(_ => newLanguage);
              }}>
              {targetPlatform
               |> getLanguagesByPlatform
               |> Belt.Array.map(_, symbol =>
                    <option value={symbol |> toLanguageString}>
                      {symbol |> toLanguageString |> React.string}
                    </option>
                  )
               |> React.array}
            </select>
          </div>
        </div>
      </Col>
    </Row>
    <VSpacing size={`px(35)} />
    /*     <div className=Styles.tableLowerContainer>
             <div className=Styles.vFlex>
               <Text value="Description" size=Text.Lg color=Colors.gray7 spacing={Text.Em(0.03)} />
             </div>
             <VSpacing size=Spacing.lg />
             <Text value=description size=Text.Lg weight=Text.Thin spacing={Text.Em(0.03)} />
           </div>
           <VSpacing size={`px(35)} /> */
    <div className=Styles.tableLowerContainer>
      <div className=Styles.vFlex>
        <img src=Images.code className=Styles.codeImage />
        <Text value="Oracle Script Schema" size=Text.Lg color=Colors.gray7 />
        <HSpacing size=Spacing.md />
        <CopyRender width=15 message=schema />
      </div>
      <VSpacing size=Spacing.lg />
      schema->renderCode
    </div>
    <GenerateDecodeCode language schema dataType=Obi.Params />
  </div>;
};
