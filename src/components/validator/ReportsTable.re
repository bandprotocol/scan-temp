module Styles = {
  open Css;

  let vFlex = align => style([display(`flex), flexDirection(`row), alignItems(align)]);

  let tableWrapper = style([padding2(~v=`px(20), ~h=`px(15))]);

  let icon = style([width(`px(80)), height(`px(80))]);
  let iconWrapper =
    style([
      width(`percent(100.)),
      display(`flex),
      flexDirection(`column),
      alignItems(`center),
    ]);

  let withWidth = w => style([width(`px(w))]);

  let fillLeft = style([marginLeft(`auto)]);

  let valueContainer =
    style([
      display(`flex),
      flexDirection(`row),
      alignItems(`flexEnd),
      paddingLeft(`px(20)),
      width(`percent(100.)),
      maxWidth(`px(170)),
      marginLeft(`auto),
    ]);

  let pagination = style([height(`px(30))]);
};

module TableHeader = {
  [@react.component]
  let make = () => {
    <THead>
      <Row>
        <Col> <HSpacing size=Spacing.md /> </Col>
        <Col size=1.>
          <Text
            block=true
            value="REQUEST"
            size=Text.Sm
            weight=Text.Semibold
            color=Colors.gray6
            spacing={Text.Em(0.05)}
          />
        </Col>
        <Col size=2.>
          <Text
            block=true
            value="TX HASH"
            size=Text.Sm
            weight=Text.Semibold
            color=Colors.gray6
            spacing={Text.Em(0.05)}
          />
        </Col>
        <Col size=2.3>
          <Text
            block=true
            value="ORACLE SCRIPT"
            size=Text.Sm
            weight=Text.Semibold
            color=Colors.gray6
            spacing={Text.Em(0.05)}
          />
        </Col>
        <Col size=1.>
          <Text
            block=true
            value="DATA SOURCE"
            size=Text.Sm
            weight=Text.Semibold
            color=Colors.gray6
            spacing={Text.Em(0.05)}
          />
        </Col>
        <Col size=0.9>
          <div className={Styles.vFlex(`flexEnd)}>
            <div className=Styles.fillLeft />
            <Text
              block=true
              value="EXTERNAL ID"
              size=Text.Sm
              weight=Text.Semibold
              color=Colors.gray6
              spacing={Text.Em(0.05)}
            />
          </div>
        </Col>
        <Col size=0.2> <HSpacing size=Spacing.sm /> </Col>
        <Col size=0.7>
          <div className={Styles.vFlex(`flexEnd)}>
            <div className=Styles.fillLeft />
            <Text
              block=true
              value="EXIT CODE"
              size=Text.Sm
              weight=Text.Semibold
              color=Colors.gray6
              spacing={Text.Em(0.05)}
            />
          </div>
        </Col>
        <Col size=2.>
          <div className={Styles.vFlex(`flexEnd)}>
            <div className=Styles.fillLeft />
            <Text
              block=true
              value="VALUE"
              size=Text.Sm
              weight=Text.Semibold
              color=Colors.gray6
              spacing={Text.Em(0.05)}
            />
          </div>
        </Col>
        <Col> <HSpacing size=Spacing.lg /> </Col>
      </Row>
    </THead>;
  };
};

module ReportCount = {
  [@react.component]
  let make = (~reportsCountSub: ApolloHooks.Subscription.variant(int)) => {
    <Row>
      {switch (reportsCountSub) {
       | Data(totalReports) =>
         <>
           <HSpacing size={`px(25)} />
           <Text value={totalReports |> string_of_int} weight=Text.Bold />
           <HSpacing size={`px(5)} />
           <Text value="Reports" />
         </>
       | _ => <LoadingCensorBar width=100 height=15 />
       }}
    </Row>;
  };
};

module Reports = {
  [@react.component]
  let make = (~reports: array(ReportSub.ValidatorReport.t)) => {
    reports
    ->Belt.Array.map(({txHash, request, reportDetails}) => {
        <TBody key={txHash |> Hash.toBase64}>
          <Row alignItems=`flexStart>
            <Col> <HSpacing size=Spacing.md /> </Col>
            <Col size=1. alignSelf=Col.Start> <TypeID.Request id={request.id} /> </Col>
            <Col size=2. alignSelf=Col.Start>
              <TxLink txHash width=150 weight=Text.Regular />
            </Col>
            <Col size=2.3 alignSelf=Col.Start>
              <Row>
                <TypeID.OracleScript id={request.oracleScript.oracleScriptID} />
                <HSpacing size=Spacing.sm />
                <HSpacing size=Spacing.xs />
                <div className={Styles.withWidth(140)}>
                  <Text value={request.oracleScript.name} block=true code=true ellipsis=true />
                </div>
              </Row>
            </Col>
            <Col size=1.>
              {reportDetails
               ->Belt_Array.map(reportDetail => {
                   <div key={reportDetail.externalID |> string_of_int}>
                     // TODO: let's investigate later why raw_request is optional.

                       <Row>
                         <TypeID.DataSource
                           id={
                                let rawRequest = reportDetail.rawRequest |> Belt_Option.getExn;
                                rawRequest.dataSourceID;
                              }
                         />
                       </Row>
                       <VSpacing size=Spacing.sm />
                       <VSpacing size=Spacing.xs />
                     </div>
                 })
               ->React.array}
            </Col>
            <Col size=0.9>
              {reportDetails
               ->Belt_Array.map(({externalID}) => {
                   <div key={externalID |> string_of_int}>
                     <div className={Styles.vFlex(`flexEnd)}>
                       <Row>
                         <div className=Styles.fillLeft />
                         <Text value={externalID |> string_of_int} block=true code=true />
                       </Row>
                     </div>
                     <VSpacing size=Spacing.md />
                   </div>
                 })
               ->React.array}
            </Col>
            <Col size=0.2> <HSpacing size=Spacing.sm /> </Col>
            <Col size=0.7>
              {reportDetails
               ->Belt_Array.map(({exitCode, externalID}) => {
                   <div key={externalID |> string_of_int}>
                     <div className={Styles.vFlex(`flexEnd)}>
                       <Row>
                         <div className=Styles.fillLeft />
                         <Text value={exitCode |> string_of_int} block=true code=true />
                       </Row>
                     </div>
                     <VSpacing size=Spacing.md />
                   </div>
                 })
               ->React.array}
            </Col>
            <Col size=2.>
              {reportDetails
               ->Belt_Array.map(({data, externalID}) => {
                   <div key={externalID |> string_of_int}>
                     <div className=Styles.valueContainer>
                       <div className=Styles.fillLeft />
                       <Text
                         value={data |> JsBuffer.toUTF8}
                         block=true
                         code=true
                         ellipsis=true
                         align=Text.Right
                       />
                     </div>
                     <VSpacing size=Spacing.md />
                   </div>
                 })
               ->React.array}
            </Col>
            <Col> <HSpacing size=Spacing.lg /> </Col>
          </Row>
        </TBody>
      })
    ->React.array;
  };
};

module Loading = {
  [@react.component]
  let make = () => {
    <>
      {Belt_Array.make(
         5,
         <Row alignItems=`flexStart>
           <Col> <HSpacing size=Spacing.md /> </Col>
           <Col size=1. alignSelf=Col.Start> <LoadingCensorBar width=50 height=20 /> </Col>
           <Col size=2. alignSelf=Col.Start> <LoadingCensorBar width=100 height=20 /> </Col>
           <Col size=2.3 alignSelf=Col.Start>
             <Row> <LoadingCensorBar width=180 height=20 /> </Row>
           </Col>
           <Col size=1.> <div> <LoadingCensorBar width=50 height=20 /> </div> </Col>
           <Col size=0.9>
             <div>
               <div className={Styles.vFlex(`flexEnd)}>
                 <Row>
                   <div className=Styles.fillLeft />
                   <LoadingCensorBar width=30 height=20 />
                 </Row>
               </div>
               <VSpacing size=Spacing.md />
             </div>
           </Col>
           <Col size=0.2> <HSpacing size=Spacing.sm /> </Col>
           <Col size=0.7>
             <div>
               <div className={Styles.vFlex(`flexEnd)}>
                 <Row>
                   <div className=Styles.fillLeft />
                   <LoadingCensorBar width=30 height=20 />
                 </Row>
               </div>
               <VSpacing size=Spacing.md />
             </div>
           </Col>
           <Col size=2.>
             <div>
               <div className=Styles.valueContainer>
                 <div className=Styles.fillLeft />
                 <LoadingCensorBar width=50 height=20 />
               </div>
               <VSpacing size=Spacing.md />
             </div>
           </Col>
           <Col> <HSpacing size=Spacing.lg /> </Col>
         </Row>,
       )
       ->Belt.Array.mapWithIndex((i, e) => {<TBody key={i |> string_of_int}> e </TBody>})
       ->React.array}
      <VSpacing size=Spacing.lg />
    </>;
  };
};

[@react.component]
let make = (~address) => {
  let (page, setPage) = React.useState(_ => 1);
  let pageSize = 5;

  let reportsSub =
    ReportSub.ValidatorReport.getListByValidator(
      ~page,
      ~pageSize,
      ~validator={
        address |> Address.toOperatorBech32;
      },
    );
  let reportsCountSub = ReportSub.ValidatorReport.count(address |> Address.toOperatorBech32);

  let allSub = Sub.all2(reportsCountSub, reportsSub);

  <div className=Styles.tableWrapper>
    <ReportCount reportsCountSub />
    <VSpacing size=Spacing.lg />
    <TableHeader />
    {switch (allSub) {
     | Data((reportsCount, reports)) =>
       let pageCount = Page.getPageCount(reportsCount, pageSize);
       reports->Belt_Array.length > 0
         ? <>
             <Reports reports />
             <VSpacing size=Spacing.lg />
             <Pagination
               currentPage=page
               pageCount
               onPageChange={newPage => setPage(_ => newPage)}
             />
           </>
         : <div className=Styles.iconWrapper>
             <VSpacing size={`px(30)} />
             <img src=Images.noRequestIcon className=Styles.icon />
             <VSpacing size={`px(40)} />
             <Text block=true value="NO REPORTS" weight=Text.Regular color=Colors.blue4 />
             <VSpacing size={`px(15)} />
           </div>;
     | _ => <Loading />
     }}
  </div>;
};
